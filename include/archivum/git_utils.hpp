#pragma once
#include <string>
#include <vector>

struct git_repository;
struct git_commit;
struct git_diff;

namespace archivum {
void init_git_subsystem();
void shutdown_git_subsystem();

struct ModifiedRange {
    uint32_t start_line;
    uint32_t end_line;
};
struct FileDiff {
    std::string file_path;
    std::vector<ModifiedRange> modified_lines;
};

class GitScanner {
   private:
    git_repository* repo;
    git_commit* lookup_commit(const std::string& sha);

   public:
    explicit GitScanner(const std::string& repo_path);
    ~GitScanner();

    GitScanner(const GitScanner&) = delete;
    GitScanner& operator=(const GitScanner&) = delete;

    std::vector<FileDiff> calculate_diff(const std::string& base_sha, const std::string& head_sha);
};
}  // namespace archivum
