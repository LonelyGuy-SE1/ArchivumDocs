![ArchivumDocs Banner](docs/assets/archivum_banner.gif)

ArchivumDocs is an AST driven, incremental documentation engine built for GitHub Actions. Rather than relying on file-level updates or manual triggers, it constructs a repository-wide structural dependency graph to minimize LLM context windows and reduce documentation drift.

Built entirely in C++20, the core engine parses commits via `libgit2` and `tree-sitter` to identify structural code modifications. By traversing the resulting adjacency list, ArchivumDocs isolates only the affected downstream components, transmitting a minimized, highly specific code context to the configured LLM provider for automated Pull Request generation.

## Architectural Overview

- **AST Native Parsing:** Uses `tree-sitter` to compile files into Abstract Syntax Trees, mapping the explicit structural boundaries of functions, classes, and namespaces.
- **Dependency Aware Incremental Updates:** Constructs an in memory directed graph of the repository. When a node (function/class) is modified, the engine traverses the graph to identify dependent components, ensuring downstream documentation is synchronized without regenerating the entire repository.
- **Zero Self Hosted Infrastructure:** Runs natively on standard GitHub Actions runners. The C++ binary executes locally, requiring zero external server hosting.
- **BYOK & Minimized Context:** Authenticates with your provided LLM API key. By utilizing the dependency graph, only the minimal required code context is transmitted to the external LLM provider, reducing token expenditure and execution latency.
