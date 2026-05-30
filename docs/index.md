---
title: "ArchivumDocs | Code Intelligence"
---

# ArchivumDocs

> AST-driven documentation that tracks code impact.

## Overview

ArchivumDocs maps code changes to documentation updates by analyzing symbols, dependency edges, and affected
call sites. The tables below highlight what changed in this range and which parts of the codebase are
impacted, so you can focus your review quickly.

## How To Read This Page

- **Changes & Impact** lists symbols directly touched by the diff.
- **Downstream Context** lists dependencies that may be affected even if they were not edited.
- **System Status** summarizes repository scale and the analysis range.

## Getting Oriented

| Symbol | Type | Status | Location |
| :--- | :--- | :--- | :--- |
| [DispatchSummary](symbols/include-archivum-api-hpp-DispatchSummary-1ed0f8fafb085122.html) | Struct | Changed | `include/archivum/api.hpp:11` |
| [ProviderRequest](symbols/include-archivum-api-hpp-ProviderRequest-27ec956435e49d66.html) | Struct | Changed | `include/archivum/api.hpp:19` |
| [ArchivumConfig](symbols/include-archivum-config-hpp-ArchivumConfig-769d8c1c99cf55c9.html) | Struct | Changed | `include/archivum/config.hpp:12` |
| [AnalysisReport](symbols/include-archivum-docs-hpp-AnalysisReport-5b292f3ad780a47b.html) | Struct | Changed | `include/archivum/docs.hpp:12` |
| [source_file_count](symbols/include-archivum-docs-hpp-source_file_count-1d371a814884025d.html) | Method | Changed | `include/archivum/docs.hpp:17` |
| [changed_file_count](symbols/include-archivum-docs-hpp-changed_file_count-6b66a3b35ad4c3d8.html) | Method | Changed | `include/archivum/docs.hpp:18` |
| [graph_node_count](symbols/include-archivum-docs-hpp-graph_node_count-6f291f24e853a01a.html) | Method | Changed | `include/archivum/docs.hpp:19` |
| [graph_edge_count](symbols/include-archivum-docs-hpp-graph_edge_count-96de4bd77b54aebb.html) | Method | Changed | `include/archivum/docs.hpp:20` |
| [DocumentationWriteResult](symbols/include-archivum-docs-hpp-DocumentationWriteResult-feb7db4834b56863.html) | Struct | Changed | `include/archivum/docs.hpp:26` |
| [ContextSymbol](symbols/include-archivum-docs-hpp-ContextSymbol-f7da82ae411d296a.html) | Struct | Changed | `include/archivum/docs.hpp:31` |
| [start_line](symbols/include-archivum-docs-hpp-start_line-85260842eb7f2df8.html) | Method | Changed | `include/archivum/docs.hpp:35` |
| [end_line](symbols/include-archivum-docs-hpp-end_line-da9369758e8049df.html) | Method | Changed | `include/archivum/docs.hpp:36` |

## Repository Map

Start here to understand the highest-activity files in this analysis window.

| File | Symbols | Changed Symbols |
| :--- | ---: | ---: |
| `src/docs.cpp` | 20 | 20 |
| `src/main.cpp` | 19 | 19 |
| `tests/test_main.cpp` | 19 | 19 |
| `src/parser.cpp` | 15 | 15 |
| `include/archivum/docs.hpp` | 14 | 14 |
| `src/api.cpp` | 12 | 12 |
| `src/graph.cpp` | 11 | 11 |
| `src/config.cpp` | 10 | 10 |

## Module Guide

High-level grouping by top-level directory.

| Module | Symbols | Changed Symbols |
| :--- | ---: | ---: |
| `src` | 97 | 97 |
| `include` | 32 | 32 |
| `tests` | 19 | 19 |

## System Status

| Statistic | Value |
| :--- | :--- |
| **Analysis Range** | `FIRST_CO` -> `afa26227` |
| **Source Files** | 14 |
| **Changed Files** | 14 |
| **Graph Density** | 148 symbols, 431 edges |
| **Impact Radius** | 148 mutated, 148 context |

## Documentation Artifacts

- [archivum-manifest.json](archivum-manifest.json) for machine-readable run metadata.
- [archivum-context-map.json](archivum-context-map.json) for a stable summary of key symbols and hotspots.

## Changes & Impact

These symbols were modified in the current diff range.

| Symbol | Type | Location |
| :--- | :--- | :--- |
| [DispatchSummary](symbols/include-archivum-api-hpp-DispatchSummary-1ed0f8fafb085122.html) | Struct | `include/archivum/api.hpp:11` |
| [ProviderRequest](symbols/include-archivum-api-hpp-ProviderRequest-27ec956435e49d66.html) | Struct | `include/archivum/api.hpp:19` |
| [ArchivumConfig](symbols/include-archivum-config-hpp-ArchivumConfig-769d8c1c99cf55c9.html) | Struct | `include/archivum/config.hpp:12` |
| [AnalysisReport](symbols/include-archivum-docs-hpp-AnalysisReport-5b292f3ad780a47b.html) | Struct | `include/archivum/docs.hpp:12` |
| [source_file_count](symbols/include-archivum-docs-hpp-source_file_count-1d371a814884025d.html) | Method | `include/archivum/docs.hpp:17` |
| [changed_file_count](symbols/include-archivum-docs-hpp-changed_file_count-6b66a3b35ad4c3d8.html) | Method | `include/archivum/docs.hpp:18` |
| [graph_node_count](symbols/include-archivum-docs-hpp-graph_node_count-6f291f24e853a01a.html) | Method | `include/archivum/docs.hpp:19` |
| [graph_edge_count](symbols/include-archivum-docs-hpp-graph_edge_count-96de4bd77b54aebb.html) | Method | `include/archivum/docs.hpp:20` |
| [DocumentationWriteResult](symbols/include-archivum-docs-hpp-DocumentationWriteResult-feb7db4834b56863.html) | Struct | `include/archivum/docs.hpp:26` |
| [ContextSymbol](symbols/include-archivum-docs-hpp-ContextSymbol-f7da82ae411d296a.html) | Struct | `include/archivum/docs.hpp:31` |
| [start_line](symbols/include-archivum-docs-hpp-start_line-85260842eb7f2df8.html) | Method | `include/archivum/docs.hpp:35` |
| [end_line](symbols/include-archivum-docs-hpp-end_line-da9369758e8049df.html) | Method | `include/archivum/docs.hpp:36` |
| [ContextFile](symbols/include-archivum-docs-hpp-ContextFile-b3e1d6606d0bebc.html) | Struct | `include/archivum/docs.hpp:39` |
| [mutated_count](symbols/include-archivum-docs-hpp-mutated_count-4dcd5fb9b9ca23cf.html) | Method | `include/archivum/docs.hpp:42` |
| [ContextMap](symbols/include-archivum-docs-hpp-ContextMap-f7ea9bda79535b84.html) | Struct | `include/archivum/docs.hpp:45` |
| [symbol_count](symbols/include-archivum-docs-hpp-symbol_count-a861d13508a9252b.html) | Method | `include/archivum/docs.hpp:46` |
| [edge_count](symbols/include-archivum-docs-hpp-edge_count-9887ac1d7dea9d32.html) | Method | `include/archivum/docs.hpp:47` |
| [git_repository](symbols/include-archivum-git_utils-hpp-git_repository-1a700ebda1181dfb.html) | Struct | `include/archivum/git_utils.hpp:7` |
| [git_commit](symbols/include-archivum-git_utils-hpp-git_commit-8572834da90d6bae.html) | Struct | `include/archivum/git_utils.hpp:8` |
| [git_diff](symbols/include-archivum-git_utils-hpp-git_diff-64e6a9d49a7beafc.html) | Struct | `include/archivum/git_utils.hpp:9` |
| [ModifiedRange](symbols/include-archivum-git_utils-hpp-ModifiedRange-900f246ade9f4e6.html) | Struct | `include/archivum/git_utils.hpp:15` |
| [FileDiff](symbols/include-archivum-git_utils-hpp-FileDiff-f2f35829c2ab853.html) | Struct | `include/archivum/git_utils.hpp:19` |
| [GitScanner](symbols/include-archivum-git_utils-hpp-GitScanner-223ad48504a54f78.html) | Method | `include/archivum/git_utils.hpp:33` |
| [operator=](symbols/include-archivum-git_utils-hpp-operator-605c95dca3e3da2d.html) | Method | `include/archivum/git_utils.hpp:34` |
| [Node](symbols/include-archivum-graph-hpp-Node-6aedf2954c5cabbc.html) | Struct | `include/archivum/graph.hpp:15` |
| [DependencyGraph](symbols/include-archivum-graph-hpp-DependencyGraph-929ff8b4239dae13.html) | Method | `include/archivum/graph.hpp:35` |
| [TSLanguage](symbols/include-archivum-parser-hpp-TSLanguage-616f17e3369c0b68.html) | Struct | `include/archivum/parser.hpp:8` |
| [TSParser](symbols/include-archivum-parser-hpp-TSParser-94d5fb5ff83cf1ad.html) | Struct | `include/archivum/parser.hpp:9` |
| [TSTree](symbols/include-archivum-parser-hpp-TSTree-713b0adce1ea6436.html) | Struct | `include/archivum/parser.hpp:10` |
| [TSNode](symbols/include-archivum-parser-hpp-TSNode-420b200d3e7c5256.html) | Struct | `include/archivum/parser.hpp:11` |
| [ASTParser](symbols/include-archivum-parser-hpp-ASTParser-563884d35118b706.html) | Method | `include/archivum/parser.hpp:31` |
| [operator=](symbols/include-archivum-parser-hpp-operator-d13abcf4ab27f382.html) | Method | `include/archivum/parser.hpp:32` |
| [present](symbols/src-api-cpp-present-f94e36293ffd0ba8.html) | Function | `src/api.cpp:15` |
| [provider_name](symbols/src-api-cpp-provider_name-8cd59e1ea405870e.html) | Function | `src/api.cpp:17` |
| [json_escape](symbols/src-api-cpp-json_escape-c1c59d539072aceb.html) | Function | `src/api.cpp:33` |
| [json_unescape](symbols/src-api-cpp-json_unescape-40fb961ba29f774c.html) | Function | `src/api.cpp:66` |
| [extract_json_string_after](symbols/src-api-cpp-extract_json_string_after-bbcabdab849a0f5a.html) | Function | `src/api.cpp:102` |
| [extract_response_text](symbols/src-api-cpp-extract_response_text-e89d7080ac28a1d2.html) | Function | `src/api.cpp:141` |
| [temp_file](symbols/src-api-cpp-temp_file-5d0ec919bd16fe62.html) | Function | `src/api.cpp:168` |
| [shell_quote](symbols/src-api-cpp-shell_quote-b21f7ba9d9353ab2.html) | Function | `src/api.cpp:173` |
| [run_capture](symbols/src-api-cpp-run_capture-4f5671d5ad46e9a3.html) | Function | `src/api.cpp:188` |
| [request_payload](symbols/src-api-cpp-request_payload-f71d4983b53d5063.html) | Function | `src/api.cpp:216` |
| [summarize_dispatch](symbols/src-api-cpp-summarize_dispatch-9d9a348da5ee1ef1.html) | Function | `src/api.cpp:235` |
| [generate_documentation_update](symbols/src-api-cpp-generate_documentation_update-594d5fb5dff27411.html) | Function | `src/api.cpp:253` |
| [trim](symbols/src-config-cpp-trim-da72ae6363f11eaf.html) | Function | `src/config.cpp:16` |
| [lower](symbols/src-config-cpp-lower-193ef47b9f304d4c.html) | Function | `src/config.cpp:23` |
| [unquote](symbols/src-config-cpp-unquote-bec0bd0daf65ed9c.html) | Function | `src/config.cpp:29` |
| [parse_bool](symbols/src-config-cpp-parse_bool-1801c6b26c7fdbfb.html) | Function | `src/config.cpp:38` |
| [parse_size](symbols/src-config-cpp-parse_size-925213239f5d1136.html) | Function | `src/config.cpp:43` |
| [parse_write_mode](symbols/src-config-cpp-parse_write_mode-e0346d91be4be660.html) | Function | `src/config.cpp:51` |
| [apply_pair](symbols/src-config-cpp-apply_pair-6b6f0288a3de8872.html) | Function | `src/config.cpp:68` |
| [apply_env](symbols/src-config-cpp-apply_env-9dd478e217b11485.html) | Function | `src/config.cpp:121` |
| [load_config](symbols/src-config-cpp-load_config-3500ff147fef8d0e.html) | Function | `src/config.cpp:161` |
| [write_mode_name](symbols/src-config-cpp-write_mode_name-e2a0b60d9a413376.html) | Function | `src/config.cpp:190` |
| [type_name](symbols/src-docs-cpp-type_name-d89f4e4a91f51824.html) | Function | `src/docs.cpp:15` |
| [type_name_sentence](symbols/src-docs-cpp-type_name_sentence-7936f936f676750.html) | Function | `src/docs.cpp:30` |
| [sanitize](symbols/src-docs-cpp-sanitize-4ecf3f46b160a02d.html) | Function | `src/docs.cpp:45` |
| [short_sha](symbols/src-docs-cpp-short_sha-9e75b1df22f76811.html) | Function | `src/docs.cpp:66` |
| [relative_link](symbols/src-docs-cpp-relative_link-2d9d81467c16029b.html) | Function | `src/docs.cpp:68` |
| [symbol_filename](symbols/src-docs-cpp-symbol_filename-ac96f6fc75c982d2.html) | Function | `src/docs.cpp:72` |
| [read_slice](symbols/src-docs-cpp-read_slice-9363414cbe0b21ff.html) | Function | `src/docs.cpp:78` |
| [write_if_changed](symbols/src-docs-cpp-write_if_changed-837e195068cff23e.html) | Function | `src/docs.cpp:111` |
| [render_symbol_page](symbols/src-docs-cpp-render_symbol_page-c72a612980583d27.html) | Function | `src/docs.cpp:127` |
| [render_index](symbols/src-docs-cpp-render_index-aa357dc0712fdf01.html) | Function | `src/docs.cpp:178` |
| [FileSummary](symbols/src-docs-cpp-FileSummary-ec598e3aa7ab071c.html) | Struct | `src/docs.cpp:246` |
| [ModuleSummary](symbols/src-docs-cpp-ModuleSummary-5396ee7f3a6b050a.html) | Struct | `src/docs.cpp:301` |
| [symbol_count](symbols/src-docs-cpp-symbol_count-9f5fb089e9d7b75e.html) | Method | `src/docs.cpp:303` |
| [mutated_count](symbols/src-docs-cpp-mutated_count-a31409fd64976414.html) | Method | `src/docs.cpp:304` |
| [render_manifest](symbols/src-docs-cpp-render_manifest-1ac65ac82346c09e.html) | Function | `src/docs.cpp:423` |
| [render_context_map](symbols/src-docs-cpp-render_context_map-74e06776a42c34f7.html) | Function | `src/docs.cpp:438` |
| [build_documentation_prompt](symbols/src-docs-cpp-build_documentation_prompt-b52de606ca75310a.html) | Function | `src/docs.cpp:479` |
| [write_documentation](symbols/src-docs-cpp-write_documentation-4997a532a20ad5f0.html) | Function | `src/docs.cpp:545` |
| [build_context_map](symbols/src-docs-cpp-build_context_map-4752917d4cc0c61b.html) | Function | `src/docs.cpp:579` |
| [write_context_map](symbols/src-docs-cpp-write_context_map-f45da8e769515a74.html) | Function | `src/docs.cpp:641` |
| [last_git_error](symbols/src-git_utils-cpp-last_git_error-621714075fb384d.html) | Function | `src/git_utils.cpp:13` |
| [oid_to_string](symbols/src-git_utils-cpp-oid_to_string-9ee07dc3a8312711.html) | Function | `src/git_utils.cpp:21` |
| [init_git_subsystem](symbols/src-git_utils-cpp-init_git_subsystem-f8f33030d36052f6.html) | Function | `src/git_utils.cpp:29` |
| [shutdown_git_subsystem](symbols/src-git_utils-cpp-shutdown_git_subsystem-c0a281cba73c5172.html) | Function | `src/git_utils.cpp:31` |
| [GitScanner::GitScanner](symbols/src-git_utils-cpp-GitScanner-GitScanner-bdf5ec967bc9266f.html) | Function | `src/git_utils.cpp:33` |
| [GitScanner::~GitScanner](symbols/src-git_utils-cpp-GitScanner-GitScanner-e5b7b096dc797693.html) | Function | `src/git_utils.cpp:41` |
| [GitScanner::lookup_commit](symbols/src-git_utils-cpp-GitScanner-lookup_commit-4850fd8f31fce8b5.html) | Function | `src/git_utils.cpp:47` |
| [GitScanner::calculate_diff](symbols/src-git_utils-cpp-GitScanner-calculate_diff-2ca5b52e1acb262b.html) | Function | `src/git_utils.cpp:63` |
| [GitScanner::resolve_reference](symbols/src-git_utils-cpp-GitScanner-resolve_reference-3fba606adcd59dd.html) | Function | `src/git_utils.cpp:146` |
| [GitScanner::parent_of](symbols/src-git_utils-cpp-GitScanner-parent_of-567630f0a7500c3b.html) | Function | `src/git_utils.cpp:164` |
| [symbol_key](symbols/src-graph-cpp-symbol_key-d739d0fa299f1451.html) | Function | `src/graph.cpp:10` |
| [DependencyGraph::register_node](symbols/src-graph-cpp-DependencyGraph-register_node-6966076afecfd172.html) | Function | `src/graph.cpp:22` |
| [DependencyGraph::add_dependency](symbols/src-graph-cpp-DependencyGraph-add_dependency-f5d715c14fa17f71.html) | Function | `src/graph.cpp:24` |
| [DependencyGraph::infer_dependencies](symbols/src-graph-cpp-DependencyGraph-infer_dependencies-de3346eb03d78d54.html) | Function | `src/graph.cpp:31` |
| [DependencyGraph::calculate_blast_radius](symbols/src-graph-cpp-DependencyGraph-calculate_blast_radius-b94cec03e193dd40.html) | Function | `src/graph.cpp:61` |
| [DependencyGraph::contains](symbols/src-graph-cpp-DependencyGraph-contains-8e33c4944f80740d.html) | Function | `src/graph.cpp:90` |
| [DependencyGraph::get_node](symbols/src-graph-cpp-DependencyGraph-get_node-467108bbdd52eb05.html) | Function | `src/graph.cpp:92` |
| [DependencyGraph::nodes](symbols/src-graph-cpp-DependencyGraph-nodes-98d046ee2a624eb9.html) | Function | `src/graph.cpp:100` |
| [DependencyGraph::downstream](symbols/src-graph-cpp-DependencyGraph-downstream-720b15ed1ff555e0.html) | Function | `src/graph.cpp:118` |
| [DependencyGraph::node_count](symbols/src-graph-cpp-DependencyGraph-node_count-37faf826bfe47edc.html) | Function | `src/graph.cpp:129` |
| [DependencyGraph::edge_count](symbols/src-graph-cpp-DependencyGraph-edge_count-1ec3ec88a06cc9f9.html) | Function | `src/graph.cpp:131` |
| [ExecutionRange](symbols/src-main-cpp-ExecutionRange-cd523c8c8cd0e1c3.html) | Struct | `src/main.cpp:26` |
| [CliOptions](symbols/src-main-cpp-CliOptions-ac26a37479e57a5c.html) | Struct | `src/main.cpp:33` |
| [~GitRuntime](symbols/src-main-cpp-GitRuntime-4e650aea64715d12.html) | Method | `src/main.cpp:41` |
| [GitRuntime](symbols/src-main-cpp-GitRuntime-ca94949e9cb07f44.html) | Method | `src/main.cpp:43` |
| [operator=](symbols/src-main-cpp-operator-3961af63434ccbd7.html) | Method | `src/main.cpp:44` |
| [is_source_file](symbols/src-main-cpp-is_source_file-55b3063e2e30d9d3.html) | Function | `src/main.cpp:47` |
| [ignored_directory](symbols/src-main-cpp-ignored_directory-34bc1af905bdd43e.html) | Function | `src/main.cpp:56` |
| [discover_source_files](symbols/src-main-cpp-discover_source_files-2877bdbd29666451.html) | Function | `src/main.cpp:61` |
| [full_repository_diffs](symbols/src-main-cpp-full_repository_diffs-d94378f7b7ac4b9.html) | Function | `src/main.cpp:87` |
| [parse_cli](symbols/src-main-cpp-parse_cli-edc58f7889b8742.html) | Function | `src/main.cpp:101` |
| [resolve_range](symbols/src-main-cpp-resolve_range-59f581b7c968cdf6.html) | Function | `src/main.cpp:133` |
| [overlaps](symbols/src-main-cpp-overlaps-8c80b54697885938.html) | Function | `src/main.cpp:160` |
| [type_name](symbols/src-main-cpp-type_name-f43d255e541b6096.html) | Function | `src/main.cpp:169` |
| [sorted_nodes](symbols/src-main-cpp-sorted_nodes-3c40411dcbc7e9a3.html) | Function | `src/main.cpp:184` |
| [print_nodes](symbols/src-main-cpp-print_nodes-ed7ec2d569c9c2a5.html) | Function | `src/main.cpp:207` |
| [cap_vector](symbols/src-main-cpp-cap_vector-fc23d06c1d6fc2b2.html) | Function | `src/main.cpp:223` |
| [provider_request](symbols/src-main-cpp-provider_request-5fc86b1c4c4704b.html) | Function | `src/main.cpp:229` |
| [run](symbols/src-main-cpp-run-6b5533aa0416b1b5.html) | Function | `src/main.cpp:242` |
| [main](symbols/src-main-cpp-main-f216d8bb99c8e1ed.html) | Function | `src/main.cpp:374` |
| [stable_hash](symbols/src-parser-cpp-stable_hash-bac1e5516bb3af8a.html) | Function | `src/parser.cpp:20` |
| [text_for_node](symbols/src-parser-cpp-text_for_node-6d0e97d14089abbe.html) | Function | `src/parser.cpp:29` |
| [normalize_space](symbols/src-parser-cpp-normalize_space-cfd25b547c1f1e54.html) | Function | `src/parser.cpp:41` |
| [is_identifier_node](symbols/src-parser-cpp-is_identifier_node-6f527ca675bcac1d.html) | Function | `src/parser.cpp:65` |
| [simple_symbol](symbols/src-parser-cpp-simple_symbol-66b699159473cde1.html) | Function | `src/parser.cpp:72` |
| [first_named_descendant](symbols/src-parser-cpp-first_named_descendant-bf121cc9b82a74fc.html) | Function | `src/parser.cpp:91` |
| [structural_name](symbols/src-parser-cpp-structural_name-2068c9894bc3139.html) | Function | `src/parser.cpp:122` |
| [has_class_ancestor](symbols/src-parser-cpp-has_class_ancestor-2875cc89619c0c7f.html) | Function | `src/parser.cpp:133` |
| [interface_text](symbols/src-parser-cpp-interface_text-8bce8a2667c84681.html) | Function | `src/parser.cpp:145` |
| [collect_references](symbols/src-parser-cpp-collect_references-36c4f86d74dfa20f.html) | Function | `src/parser.cpp:166` |
| [ASTParser::ASTParser](symbols/src-parser-cpp-ASTParser-ASTParser-658f8f1d8ac4cef8.html) | Function | `src/parser.cpp:183` |
| [ASTParser::~ASTParser](symbols/src-parser-cpp-ASTParser-ASTParser-c5216ab191af5abc.html) | Function | `src/parser.cpp:190` |
| [ASTParser::generate_node_id](symbols/src-parser-cpp-ASTParser-generate_node_id-51853e208f082d39.html) | Function | `src/parser.cpp:196` |
| [ASTParser::parse_file](symbols/src-parser-cpp-ASTParser-parse_file-3a00561966f79bd3.html) | Function | `src/parser.cpp:201` |
| [ASTParser::extract_structural_nodes](symbols/src-parser-cpp-ASTParser-extract_structural_nodes-b90e6df760b55a6.html) | Function | `src/parser.cpp:236` |
| [ScopedTemp](symbols/tests-test_main-cpp-ScopedTemp-e7726f9bf318bb30.html) | Method | `tests/test_main.cpp:26` |
| [~ScopedTemp](symbols/tests-test_main-cpp-ScopedTemp-8d9d170183563ea6.html) | Method | `tests/test_main.cpp:32` |
| [path](symbols/tests-test_main-cpp-path-7c55d1556c585c45.html) | Method | `tests/test_main.cpp:39` |
| [require](symbols/tests-test_main-cpp-require-f7e7844e0b4aba59.html) | Function | `tests/test_main.cpp:45` |
| [write_file](symbols/tests-test_main-cpp-write_file-dfeac45df68bbc84.html) | Function | `tests/test_main.cpp:51` |
| [quote](symbols/tests-test_main-cpp-quote-e2a54783a9a7ef2.html) | Function | `tests/test_main.cpp:58` |
| [run_command](symbols/tests-test_main-cpp-run_command-87da30975a9ad6e1.html) | Function | `tests/test_main.cpp:60` |
| [make_node](symbols/tests-test_main-cpp-make_node-537bb11f94b16199.html) | Function | `tests/test_main.cpp:65` |
| [find_node](symbols/tests-test_main-cpp-find_node-916503013ac3b41e.html) | Function | `tests/test_main.cpp:80` |
| [parser_extracts_structural_metadata](symbols/tests-test_main-cpp-parser_extracts_structural_metadata-24dcb0b3148e2226.html) | Function | `tests/test_main.cpp:85` |
| [graph_infers_transitive_impact](symbols/tests-test_main-cpp-graph_infers_transitive_impact-fb6261684b70c163.html) | Function | `tests/test_main.cpp:121` |
| [graph_handles_cycles](symbols/tests-test_main-cpp-graph_handles_cycles-a12939dc430f1884.html) | Function | `tests/test_main.cpp:133` |
| [git_scanner_reports_changed_ranges](symbols/tests-test_main-cpp-git_scanner_reports_changed_ranges-f203769c2ca349df.html) | Function | `tests/test_main.cpp:146` |
| [config_loads_defaults_and_overrides](symbols/tests-test_main-cpp-config_loads_defaults_and_overrides-f685b66d9be0d461.html) | Function | `tests/test_main.cpp:188` |
| [context_map_writer_is_idempotent](symbols/tests-test_main-cpp-context_map_writer_is_idempotent-569531a6e1a083c3.html) | Function | `tests/test_main.cpp:206` |
| [documentation_writer_creates_index_symbols_and_manifest](symbols/tests-test_main-cpp-documentation_writer_creates_index_symbols_and_manifest-c098c92a00b483d.html) | Function | `tests/test_main.cpp:250` |
| [provider_skips_when_key_is_absent](symbols/tests-test_main-cpp-provider_skips_when_key_is_absent-2999a816b4e29c32.html) | Function | `tests/test_main.cpp:298` |
| [run_test](symbols/tests-test_main-cpp-run_test-f3b0ab317b9a020c.html) | Function | `tests/test_main.cpp:312` |
| [main](symbols/tests-test_main-cpp-main-60d84673108ffa27.html) | Function | `tests/test_main.cpp:319` |

## Downstream Context

These dependencies may be affected by the changes above.

| Symbol | Type | Relationship |
| :--- | :--- | :--- |
| [DispatchSummary](symbols/include-archivum-api-hpp-DispatchSummary-1ed0f8fafb085122.html) | Struct | Downstream dependency |
| [ProviderRequest](symbols/include-archivum-api-hpp-ProviderRequest-27ec956435e49d66.html) | Struct | Downstream dependency |
| [ArchivumConfig](symbols/include-archivum-config-hpp-ArchivumConfig-769d8c1c99cf55c9.html) | Struct | Downstream dependency |
| [AnalysisReport](symbols/include-archivum-docs-hpp-AnalysisReport-5b292f3ad780a47b.html) | Struct | Downstream dependency |
| [source_file_count](symbols/include-archivum-docs-hpp-source_file_count-1d371a814884025d.html) | Method | Downstream dependency |
| [changed_file_count](symbols/include-archivum-docs-hpp-changed_file_count-6b66a3b35ad4c3d8.html) | Method | Downstream dependency |
| [graph_node_count](symbols/include-archivum-docs-hpp-graph_node_count-6f291f24e853a01a.html) | Method | Downstream dependency |
| [graph_edge_count](symbols/include-archivum-docs-hpp-graph_edge_count-96de4bd77b54aebb.html) | Method | Downstream dependency |
| [DocumentationWriteResult](symbols/include-archivum-docs-hpp-DocumentationWriteResult-feb7db4834b56863.html) | Struct | Downstream dependency |
| [ContextSymbol](symbols/include-archivum-docs-hpp-ContextSymbol-f7da82ae411d296a.html) | Struct | Downstream dependency |
| [start_line](symbols/include-archivum-docs-hpp-start_line-85260842eb7f2df8.html) | Method | Downstream dependency |
| [end_line](symbols/include-archivum-docs-hpp-end_line-da9369758e8049df.html) | Method | Downstream dependency |
| [ContextFile](symbols/include-archivum-docs-hpp-ContextFile-b3e1d6606d0bebc.html) | Struct | Downstream dependency |
| [mutated_count](symbols/include-archivum-docs-hpp-mutated_count-4dcd5fb9b9ca23cf.html) | Method | Downstream dependency |
| [ContextMap](symbols/include-archivum-docs-hpp-ContextMap-f7ea9bda79535b84.html) | Struct | Downstream dependency |
| [symbol_count](symbols/include-archivum-docs-hpp-symbol_count-a861d13508a9252b.html) | Method | Downstream dependency |
| [edge_count](symbols/include-archivum-docs-hpp-edge_count-9887ac1d7dea9d32.html) | Method | Downstream dependency |
| [git_repository](symbols/include-archivum-git_utils-hpp-git_repository-1a700ebda1181dfb.html) | Struct | Downstream dependency |
| [git_commit](symbols/include-archivum-git_utils-hpp-git_commit-8572834da90d6bae.html) | Struct | Downstream dependency |
| [git_diff](symbols/include-archivum-git_utils-hpp-git_diff-64e6a9d49a7beafc.html) | Struct | Downstream dependency |
| [ModifiedRange](symbols/include-archivum-git_utils-hpp-ModifiedRange-900f246ade9f4e6.html) | Struct | Downstream dependency |
| [FileDiff](symbols/include-archivum-git_utils-hpp-FileDiff-f2f35829c2ab853.html) | Struct | Downstream dependency |
| [GitScanner](symbols/include-archivum-git_utils-hpp-GitScanner-223ad48504a54f78.html) | Method | Downstream dependency |
| [operator=](symbols/include-archivum-git_utils-hpp-operator-605c95dca3e3da2d.html) | Method | Downstream dependency |
| [Node](symbols/include-archivum-graph-hpp-Node-6aedf2954c5cabbc.html) | Struct | Downstream dependency |
| [DependencyGraph](symbols/include-archivum-graph-hpp-DependencyGraph-929ff8b4239dae13.html) | Method | Downstream dependency |
| [TSLanguage](symbols/include-archivum-parser-hpp-TSLanguage-616f17e3369c0b68.html) | Struct | Downstream dependency |
| [TSParser](symbols/include-archivum-parser-hpp-TSParser-94d5fb5ff83cf1ad.html) | Struct | Downstream dependency |
| [TSTree](symbols/include-archivum-parser-hpp-TSTree-713b0adce1ea6436.html) | Struct | Downstream dependency |
| [TSNode](symbols/include-archivum-parser-hpp-TSNode-420b200d3e7c5256.html) | Struct | Downstream dependency |
| [ASTParser](symbols/include-archivum-parser-hpp-ASTParser-563884d35118b706.html) | Method | Downstream dependency |
| [operator=](symbols/include-archivum-parser-hpp-operator-d13abcf4ab27f382.html) | Method | Downstream dependency |
| [present](symbols/src-api-cpp-present-f94e36293ffd0ba8.html) | Function | Downstream dependency |
| [provider_name](symbols/src-api-cpp-provider_name-8cd59e1ea405870e.html) | Function | Downstream dependency |
| [json_escape](symbols/src-api-cpp-json_escape-c1c59d539072aceb.html) | Function | Downstream dependency |
| [json_unescape](symbols/src-api-cpp-json_unescape-40fb961ba29f774c.html) | Function | Downstream dependency |
| [extract_json_string_after](symbols/src-api-cpp-extract_json_string_after-bbcabdab849a0f5a.html) | Function | Downstream dependency |
| [extract_response_text](symbols/src-api-cpp-extract_response_text-e89d7080ac28a1d2.html) | Function | Downstream dependency |
| [temp_file](symbols/src-api-cpp-temp_file-5d0ec919bd16fe62.html) | Function | Downstream dependency |
| [shell_quote](symbols/src-api-cpp-shell_quote-b21f7ba9d9353ab2.html) | Function | Downstream dependency |
| [run_capture](symbols/src-api-cpp-run_capture-4f5671d5ad46e9a3.html) | Function | Downstream dependency |
| [request_payload](symbols/src-api-cpp-request_payload-f71d4983b53d5063.html) | Function | Downstream dependency |
| [summarize_dispatch](symbols/src-api-cpp-summarize_dispatch-9d9a348da5ee1ef1.html) | Function | Downstream dependency |
| [generate_documentation_update](symbols/src-api-cpp-generate_documentation_update-594d5fb5dff27411.html) | Function | Downstream dependency |
| [trim](symbols/src-config-cpp-trim-da72ae6363f11eaf.html) | Function | Downstream dependency |
| [lower](symbols/src-config-cpp-lower-193ef47b9f304d4c.html) | Function | Downstream dependency |
| [unquote](symbols/src-config-cpp-unquote-bec0bd0daf65ed9c.html) | Function | Downstream dependency |
| [parse_bool](symbols/src-config-cpp-parse_bool-1801c6b26c7fdbfb.html) | Function | Downstream dependency |
| [parse_size](symbols/src-config-cpp-parse_size-925213239f5d1136.html) | Function | Downstream dependency |
| [parse_write_mode](symbols/src-config-cpp-parse_write_mode-e0346d91be4be660.html) | Function | Downstream dependency |
| [apply_pair](symbols/src-config-cpp-apply_pair-6b6f0288a3de8872.html) | Function | Downstream dependency |
| [apply_env](symbols/src-config-cpp-apply_env-9dd478e217b11485.html) | Function | Downstream dependency |
| [load_config](symbols/src-config-cpp-load_config-3500ff147fef8d0e.html) | Function | Downstream dependency |
| [write_mode_name](symbols/src-config-cpp-write_mode_name-e2a0b60d9a413376.html) | Function | Downstream dependency |
| [type_name](symbols/src-docs-cpp-type_name-d89f4e4a91f51824.html) | Function | Downstream dependency |
| [type_name_sentence](symbols/src-docs-cpp-type_name_sentence-7936f936f676750.html) | Function | Downstream dependency |
| [sanitize](symbols/src-docs-cpp-sanitize-4ecf3f46b160a02d.html) | Function | Downstream dependency |
| [short_sha](symbols/src-docs-cpp-short_sha-9e75b1df22f76811.html) | Function | Downstream dependency |
| [relative_link](symbols/src-docs-cpp-relative_link-2d9d81467c16029b.html) | Function | Downstream dependency |
| [symbol_filename](symbols/src-docs-cpp-symbol_filename-ac96f6fc75c982d2.html) | Function | Downstream dependency |
| [read_slice](symbols/src-docs-cpp-read_slice-9363414cbe0b21ff.html) | Function | Downstream dependency |
| [write_if_changed](symbols/src-docs-cpp-write_if_changed-837e195068cff23e.html) | Function | Downstream dependency |
| [render_symbol_page](symbols/src-docs-cpp-render_symbol_page-c72a612980583d27.html) | Function | Downstream dependency |
| [render_index](symbols/src-docs-cpp-render_index-aa357dc0712fdf01.html) | Function | Downstream dependency |
| [FileSummary](symbols/src-docs-cpp-FileSummary-ec598e3aa7ab071c.html) | Struct | Downstream dependency |
| [ModuleSummary](symbols/src-docs-cpp-ModuleSummary-5396ee7f3a6b050a.html) | Struct | Downstream dependency |
| [symbol_count](symbols/src-docs-cpp-symbol_count-9f5fb089e9d7b75e.html) | Method | Downstream dependency |
| [mutated_count](symbols/src-docs-cpp-mutated_count-a31409fd64976414.html) | Method | Downstream dependency |
| [render_manifest](symbols/src-docs-cpp-render_manifest-1ac65ac82346c09e.html) | Function | Downstream dependency |
| [render_context_map](symbols/src-docs-cpp-render_context_map-74e06776a42c34f7.html) | Function | Downstream dependency |
| [build_documentation_prompt](symbols/src-docs-cpp-build_documentation_prompt-b52de606ca75310a.html) | Function | Downstream dependency |
| [write_documentation](symbols/src-docs-cpp-write_documentation-4997a532a20ad5f0.html) | Function | Downstream dependency |
| [build_context_map](symbols/src-docs-cpp-build_context_map-4752917d4cc0c61b.html) | Function | Downstream dependency |
| [write_context_map](symbols/src-docs-cpp-write_context_map-f45da8e769515a74.html) | Function | Downstream dependency |
| [last_git_error](symbols/src-git_utils-cpp-last_git_error-621714075fb384d.html) | Function | Downstream dependency |
| [oid_to_string](symbols/src-git_utils-cpp-oid_to_string-9ee07dc3a8312711.html) | Function | Downstream dependency |
| [init_git_subsystem](symbols/src-git_utils-cpp-init_git_subsystem-f8f33030d36052f6.html) | Function | Downstream dependency |
| [shutdown_git_subsystem](symbols/src-git_utils-cpp-shutdown_git_subsystem-c0a281cba73c5172.html) | Function | Downstream dependency |
| [GitScanner::GitScanner](symbols/src-git_utils-cpp-GitScanner-GitScanner-bdf5ec967bc9266f.html) | Function | Downstream dependency |
| [GitScanner::~GitScanner](symbols/src-git_utils-cpp-GitScanner-GitScanner-e5b7b096dc797693.html) | Function | Downstream dependency |
| [GitScanner::lookup_commit](symbols/src-git_utils-cpp-GitScanner-lookup_commit-4850fd8f31fce8b5.html) | Function | Downstream dependency |
| [GitScanner::calculate_diff](symbols/src-git_utils-cpp-GitScanner-calculate_diff-2ca5b52e1acb262b.html) | Function | Downstream dependency |
| [GitScanner::resolve_reference](symbols/src-git_utils-cpp-GitScanner-resolve_reference-3fba606adcd59dd.html) | Function | Downstream dependency |
| [GitScanner::parent_of](symbols/src-git_utils-cpp-GitScanner-parent_of-567630f0a7500c3b.html) | Function | Downstream dependency |
| [symbol_key](symbols/src-graph-cpp-symbol_key-d739d0fa299f1451.html) | Function | Downstream dependency |
| [DependencyGraph::register_node](symbols/src-graph-cpp-DependencyGraph-register_node-6966076afecfd172.html) | Function | Downstream dependency |
| [DependencyGraph::add_dependency](symbols/src-graph-cpp-DependencyGraph-add_dependency-f5d715c14fa17f71.html) | Function | Downstream dependency |
| [DependencyGraph::infer_dependencies](symbols/src-graph-cpp-DependencyGraph-infer_dependencies-de3346eb03d78d54.html) | Function | Downstream dependency |
| [DependencyGraph::calculate_blast_radius](symbols/src-graph-cpp-DependencyGraph-calculate_blast_radius-b94cec03e193dd40.html) | Function | Downstream dependency |
| [DependencyGraph::contains](symbols/src-graph-cpp-DependencyGraph-contains-8e33c4944f80740d.html) | Function | Downstream dependency |
| [DependencyGraph::get_node](symbols/src-graph-cpp-DependencyGraph-get_node-467108bbdd52eb05.html) | Function | Downstream dependency |
| [DependencyGraph::nodes](symbols/src-graph-cpp-DependencyGraph-nodes-98d046ee2a624eb9.html) | Function | Downstream dependency |
| [DependencyGraph::downstream](symbols/src-graph-cpp-DependencyGraph-downstream-720b15ed1ff555e0.html) | Function | Downstream dependency |
| [DependencyGraph::node_count](symbols/src-graph-cpp-DependencyGraph-node_count-37faf826bfe47edc.html) | Function | Downstream dependency |
| [DependencyGraph::edge_count](symbols/src-graph-cpp-DependencyGraph-edge_count-1ec3ec88a06cc9f9.html) | Function | Downstream dependency |
| [ExecutionRange](symbols/src-main-cpp-ExecutionRange-cd523c8c8cd0e1c3.html) | Struct | Downstream dependency |
| [CliOptions](symbols/src-main-cpp-CliOptions-ac26a37479e57a5c.html) | Struct | Downstream dependency |
| [~GitRuntime](symbols/src-main-cpp-GitRuntime-4e650aea64715d12.html) | Method | Downstream dependency |
| [GitRuntime](symbols/src-main-cpp-GitRuntime-ca94949e9cb07f44.html) | Method | Downstream dependency |
| [operator=](symbols/src-main-cpp-operator-3961af63434ccbd7.html) | Method | Downstream dependency |
| [is_source_file](symbols/src-main-cpp-is_source_file-55b3063e2e30d9d3.html) | Function | Downstream dependency |
| [ignored_directory](symbols/src-main-cpp-ignored_directory-34bc1af905bdd43e.html) | Function | Downstream dependency |
| [discover_source_files](symbols/src-main-cpp-discover_source_files-2877bdbd29666451.html) | Function | Downstream dependency |
| [full_repository_diffs](symbols/src-main-cpp-full_repository_diffs-d94378f7b7ac4b9.html) | Function | Downstream dependency |
| [parse_cli](symbols/src-main-cpp-parse_cli-edc58f7889b8742.html) | Function | Downstream dependency |
| [resolve_range](symbols/src-main-cpp-resolve_range-59f581b7c968cdf6.html) | Function | Downstream dependency |
| [overlaps](symbols/src-main-cpp-overlaps-8c80b54697885938.html) | Function | Downstream dependency |
| [type_name](symbols/src-main-cpp-type_name-f43d255e541b6096.html) | Function | Downstream dependency |
| [sorted_nodes](symbols/src-main-cpp-sorted_nodes-3c40411dcbc7e9a3.html) | Function | Downstream dependency |
| [print_nodes](symbols/src-main-cpp-print_nodes-ed7ec2d569c9c2a5.html) | Function | Downstream dependency |
| [cap_vector](symbols/src-main-cpp-cap_vector-fc23d06c1d6fc2b2.html) | Function | Downstream dependency |
| [provider_request](symbols/src-main-cpp-provider_request-5fc86b1c4c4704b.html) | Function | Downstream dependency |
| [run](symbols/src-main-cpp-run-6b5533aa0416b1b5.html) | Function | Downstream dependency |
| [main](symbols/src-main-cpp-main-f216d8bb99c8e1ed.html) | Function | Downstream dependency |
| [stable_hash](symbols/src-parser-cpp-stable_hash-bac1e5516bb3af8a.html) | Function | Downstream dependency |
| [text_for_node](symbols/src-parser-cpp-text_for_node-6d0e97d14089abbe.html) | Function | Downstream dependency |
| [normalize_space](symbols/src-parser-cpp-normalize_space-cfd25b547c1f1e54.html) | Function | Downstream dependency |
| [is_identifier_node](symbols/src-parser-cpp-is_identifier_node-6f527ca675bcac1d.html) | Function | Downstream dependency |
| [simple_symbol](symbols/src-parser-cpp-simple_symbol-66b699159473cde1.html) | Function | Downstream dependency |
| [first_named_descendant](symbols/src-parser-cpp-first_named_descendant-bf121cc9b82a74fc.html) | Function | Downstream dependency |
| [structural_name](symbols/src-parser-cpp-structural_name-2068c9894bc3139.html) | Function | Downstream dependency |
| [has_class_ancestor](symbols/src-parser-cpp-has_class_ancestor-2875cc89619c0c7f.html) | Function | Downstream dependency |
| [interface_text](symbols/src-parser-cpp-interface_text-8bce8a2667c84681.html) | Function | Downstream dependency |
| [collect_references](symbols/src-parser-cpp-collect_references-36c4f86d74dfa20f.html) | Function | Downstream dependency |
| [ASTParser::ASTParser](symbols/src-parser-cpp-ASTParser-ASTParser-658f8f1d8ac4cef8.html) | Function | Downstream dependency |
| [ASTParser::~ASTParser](symbols/src-parser-cpp-ASTParser-ASTParser-c5216ab191af5abc.html) | Function | Downstream dependency |
| [ASTParser::generate_node_id](symbols/src-parser-cpp-ASTParser-generate_node_id-51853e208f082d39.html) | Function | Downstream dependency |
| [ASTParser::parse_file](symbols/src-parser-cpp-ASTParser-parse_file-3a00561966f79bd3.html) | Function | Downstream dependency |
| [ASTParser::extract_structural_nodes](symbols/src-parser-cpp-ASTParser-extract_structural_nodes-b90e6df760b55a6.html) | Function | Downstream dependency |
| [ScopedTemp](symbols/tests-test_main-cpp-ScopedTemp-e7726f9bf318bb30.html) | Method | Downstream dependency |
| [~ScopedTemp](symbols/tests-test_main-cpp-ScopedTemp-8d9d170183563ea6.html) | Method | Downstream dependency |
| [path](symbols/tests-test_main-cpp-path-7c55d1556c585c45.html) | Method | Downstream dependency |
| [require](symbols/tests-test_main-cpp-require-f7e7844e0b4aba59.html) | Function | Downstream dependency |
| [write_file](symbols/tests-test_main-cpp-write_file-dfeac45df68bbc84.html) | Function | Downstream dependency |
| [quote](symbols/tests-test_main-cpp-quote-e2a54783a9a7ef2.html) | Function | Downstream dependency |
| [run_command](symbols/tests-test_main-cpp-run_command-87da30975a9ad6e1.html) | Function | Downstream dependency |
| [make_node](symbols/tests-test_main-cpp-make_node-537bb11f94b16199.html) | Function | Downstream dependency |
| [find_node](symbols/tests-test_main-cpp-find_node-916503013ac3b41e.html) | Function | Downstream dependency |
| [parser_extracts_structural_metadata](symbols/tests-test_main-cpp-parser_extracts_structural_metadata-24dcb0b3148e2226.html) | Function | Downstream dependency |
| [graph_infers_transitive_impact](symbols/tests-test_main-cpp-graph_infers_transitive_impact-fb6261684b70c163.html) | Function | Downstream dependency |
| [graph_handles_cycles](symbols/tests-test_main-cpp-graph_handles_cycles-a12939dc430f1884.html) | Function | Downstream dependency |
| [git_scanner_reports_changed_ranges](symbols/tests-test_main-cpp-git_scanner_reports_changed_ranges-f203769c2ca349df.html) | Function | Downstream dependency |
| [config_loads_defaults_and_overrides](symbols/tests-test_main-cpp-config_loads_defaults_and_overrides-f685b66d9be0d461.html) | Function | Downstream dependency |
| [context_map_writer_is_idempotent](symbols/tests-test_main-cpp-context_map_writer_is_idempotent-569531a6e1a083c3.html) | Function | Downstream dependency |
| [documentation_writer_creates_index_symbols_and_manifest](symbols/tests-test_main-cpp-documentation_writer_creates_index_symbols_and_manifest-c098c92a00b483d.html) | Function | Downstream dependency |
| [provider_skips_when_key_is_absent](symbols/tests-test_main-cpp-provider_skips_when_key_is_absent-2999a816b4e29c32.html) | Function | Downstream dependency |
| [run_test](symbols/tests-test_main-cpp-run_test-f3b0ab317b9a020c.html) | Function | Downstream dependency |
| [main](symbols/tests-test_main-cpp-main-60d84673108ffa27.html) | Function | Downstream dependency |
