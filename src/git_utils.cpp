#include <git2.h>

#include <archivum/git_utils.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

namespace archivum {

namespace {

std::string last_git_error(const std::string& action) {
    const git_error* error = git_error_last();
    if (error != nullptr && error->message != nullptr) {
        return "[ArchivumDocs] FATAL: " + action + ": " + error->message;
    }
    return "[ArchivumDocs] FATAL: " + action + ".";
}

std::string oid_to_string(const git_oid* oid) {
    char value[GIT_OID_HEXSZ + 1];
    git_oid_tostr(value, sizeof(value), oid);
    return value;
}

}  // namespace

void init_git_subsystem() { git_libgit2_init(); }

void shutdown_git_subsystem() { git_libgit2_shutdown(); }

GitScanner::GitScanner(const std::string& repo_path) {
    repo = nullptr;
    int error = git_repository_open(&repo, repo_path.c_str());
    if (error < 0) {
        throw std::runtime_error(last_git_error("Failed to open Git repository at " + repo_path));
    }
}

GitScanner::~GitScanner() {
    if (repo != nullptr) {
        git_repository_free(repo);
    }
}

git_commit* GitScanner::lookup_commit(const std::string& sha) const {
    git_oid oid;
    int error = git_oid_fromstr(&oid, sha.c_str());
    if (error < 0) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Invalid SHA format: " + sha);
    }

    git_commit* commit = nullptr;
    error = git_commit_lookup(&commit, repo, &oid);
    if (error < 0) {
        throw std::runtime_error(last_git_error("Commit SHA not found in storage: " + sha));
    }

    return commit;
}

std::vector<FileDiff> GitScanner::calculate_diff(const std::string& base_sha, const std::string& head_sha) {
    std::vector<FileDiff> results;
    using CommitPtr = std::unique_ptr<git_commit, decltype(&git_commit_free)>;
    using TreePtr = std::unique_ptr<git_tree, decltype(&git_tree_free)>;
    using DiffPtr = std::unique_ptr<git_diff, decltype(&git_diff_free)>;
    using PatchPtr = std::unique_ptr<git_patch, decltype(&git_patch_free)>;

    CommitPtr base_commit(lookup_commit(base_sha), git_commit_free);
    CommitPtr head_commit(lookup_commit(head_sha), git_commit_free);

    git_tree* base_tree = nullptr;
    git_tree* head_tree = nullptr;

    if (git_commit_tree(&base_tree, base_commit.get()) < 0) {
        throw std::runtime_error(last_git_error("Failed to read base commit tree"));
    }
    TreePtr base_tree_guard(base_tree, git_tree_free);

    if (git_commit_tree(&head_tree, head_commit.get()) < 0) {
        throw std::runtime_error(last_git_error("Failed to read head commit tree"));
    }
    TreePtr head_tree_guard(head_tree, git_tree_free);

    git_diff_options options = GIT_DIFF_OPTIONS_INIT;
    options.flags = GIT_DIFF_INCLUDE_TYPECHANGE | GIT_DIFF_RECURSE_UNTRACKED_DIRS;

    git_diff* diff = nullptr;
    int error = git_diff_tree_to_tree(&diff, repo, base_tree_guard.get(), head_tree_guard.get(), &options);
    if (error < 0) {
        throw std::runtime_error(last_git_error("Failed to calculate Git diff"));
    }

    DiffPtr diff_guard(diff, git_diff_free);
    size_t num_deltas = git_diff_num_deltas(diff_guard.get());

    for (size_t i = 0; i < num_deltas; ++i) {
        const git_diff_delta* delta = git_diff_get_delta(diff_guard.get(), i);
        if (delta == nullptr) {
            continue;
        }

        if (delta->status != GIT_DELTA_MODIFIED && delta->status != GIT_DELTA_ADDED &&
            delta->status != GIT_DELTA_RENAMED && delta->status != GIT_DELTA_COPIED &&
            delta->status != GIT_DELTA_TYPECHANGE) {
            continue;
        }

        FileDiff file_diff;
        file_diff.file_path = delta->new_file.path == nullptr ? "" : delta->new_file.path;

        git_patch* patch = nullptr;
        if (git_patch_from_diff(&patch, diff_guard.get(), i) < 0) {
            throw std::runtime_error(last_git_error("Failed to inspect patch for " + file_diff.file_path));
        }
        PatchPtr patch_guard(patch, git_patch_free);

        if (patch_guard != nullptr) {
            size_t num_hunks = git_patch_num_hunks(patch_guard.get());
            for (size_t j = 0; j < num_hunks; ++j) {
                const git_diff_hunk* hunk = nullptr;
                size_t lines_in_hunk = 0;
                if (git_patch_get_hunk(&hunk, &lines_in_hunk, patch_guard.get(), j) < 0 || hunk == nullptr) {
                    continue;
                }

                uint32_t start = hunk->new_start > 0 ? static_cast<uint32_t>(hunk->new_start) : 1;
                uint32_t span = hunk->new_lines > 0 ? static_cast<uint32_t>(hunk->new_lines) : 1;
                file_diff.modified_lines.push_back({start, start + span - 1});
            }
        }

        if (file_diff.modified_lines.empty() && !file_diff.file_path.empty()) {
            file_diff.modified_lines.push_back({1, 1});
        }

        if (!file_diff.file_path.empty()) {
            results.push_back(std::move(file_diff));
        }
    }

    return results;
}

std::string GitScanner::resolve_reference(const std::string& reference) const {
    git_object* object = nullptr;
    int error = git_revparse_single(&object, repo, reference.c_str());
    if (error < 0 || object == nullptr) {
        throw std::runtime_error(last_git_error("Failed to resolve Git reference " + reference));
    }

    const git_oid* oid = git_object_id(object);
    if (oid == nullptr) {
        git_object_free(object);
        throw std::runtime_error("[ArchivumDocs] FATAL: Git reference does not resolve to an object: " + reference);
    }

    std::string value = oid_to_string(oid);
    git_object_free(object);
    return value;
}

std::optional<std::string> GitScanner::parent_of(const std::string& sha) const {
    using CommitPtr = std::unique_ptr<git_commit, decltype(&git_commit_free)>;
    CommitPtr commit(lookup_commit(sha), git_commit_free);

    if (git_commit_parentcount(commit.get()) == 0) {
        return std::nullopt;
    }

    git_commit* parent = nullptr;
    if (git_commit_parent(&parent, commit.get(), 0) < 0) {
        throw std::runtime_error(last_git_error("Failed to resolve parent commit for " + sha));
    }
    CommitPtr parent_guard(parent, git_commit_free);

    return oid_to_string(git_commit_id(parent_guard.get()));
}

}  // namespace archivum
