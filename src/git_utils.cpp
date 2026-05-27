#include <git2.h>

#include <archivum/git_utils.hpp>
#include <iostream>
#include <stdexcept>

namespace archivum {

void init_git_subsystem() { git_libgit2_init(); }

void shutdown_git_subsystem() { git_libgit2_shutdown(); }

GitScanner::GitScanner(const std::string& repo_path) {
    repo = nullptr;
    int error = git_repository_open(&repo, repo_path.c_str());
    if (error < 0) {
        throw std::runtime_error("[ArchivumDocs] FATAL: Failed to open physical .git repository at " + repo_path);
    }
}

GitScanner::~GitScanner() {
    if (repo != nullptr) {
        git_repository_free(repo);
    }
}

git_commit* GitScanner::lookup_commit(const std::string& sha) {
    git_oid oid;
    int error = git_oid_fromstr(&oid, sha.c_str());
    if (error < 0) {
        std::cerr << "[ArchivumDocs] ERROR: Invalid SHA format: " << sha << std::endl;
        return nullptr;
    }

    git_commit* commit = nullptr;
    error = git_commit_lookup(&commit, repo, &oid);
    if (error < 0) {
        std::cerr << "[ArchivumDocs] ERROR: Commit SHA not found in storage: " << sha << std::endl;
        return nullptr;
    }

    return commit;
}

std::vector<FileDiff> GitScanner::calculate_diff(const std::string& base_sha, const std::string& head_sha) {
    std::vector<FileDiff> results;

    git_commit* base_commit = lookup_commit(base_sha);
    git_commit* head_commit = lookup_commit(head_sha);

    if (!base_commit || !head_commit) {
        if (base_commit)
            git_commit_free(base_commit);
        if (head_commit)
            git_commit_free(head_commit);
        return results;
    }

    git_tree* base_tree = nullptr;
    git_tree* head_tree = nullptr;

    git_commit_tree(&base_tree, base_commit);
    git_commit_tree(&head_tree, head_commit);

    git_diff* diff = nullptr;

    int error = git_diff_tree_to_tree(&diff, repo, base_tree, head_tree, nullptr);

    if (error == 0 && diff != nullptr) {
        size_t num_deltas = git_diff_num_deltas(diff);

        for (size_t i = 0; i < num_deltas; ++i) {
            const git_diff_delta* delta = git_diff_get_delta(diff, i);
            if (delta->status == GIT_DELTA_MODIFIED || delta->status == GIT_DELTA_ADDED) {
                FileDiff file_diff;
                file_diff.file_path = delta->new_file.path;

                git_patch* patch = nullptr;
                git_patch_from_diff(&patch, diff, i);

                if (patch != nullptr) {
                    size_t num_hunks = git_patch_num_hunks(patch);
                    for (size_t j = 0; j < num_hunks; ++j) {
                        const git_diff_hunk* hunk = nullptr;
                        size_t lines_in_hunk = 0;
                        git_patch_get_hunk(&hunk, &lines_in_hunk, patch, j);

                        if (hunk != nullptr) {
                            ModifiedRange range;
                            range.start_line = hunk->new_start;
                            range.end_line = hunk->new_start + hunk->new_lines - 1;
                            file_diff.modified_lines.push_back(range);
                        }
                    }
                    git_patch_free(patch);
                }
                results.push_back(file_diff);
            }
        }
        git_diff_free(diff);
    }

    git_tree_free(base_tree);
    git_tree_free(head_tree);
    git_commit_free(base_commit);
    git_commit_free(head_commit);

    return results;
}

}  // namespace archivum