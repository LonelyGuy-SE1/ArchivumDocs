#include <algorithm>
#include <archivum/api.hpp>
#include <archivum/config.hpp>
#include <archivum/docs.hpp>
#include <archivum/git_utils.hpp>
#include <archivum/graph.hpp>
#include <archivum/parser.hpp>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

class ScopedTemp {
   public:
    ScopedTemp() {
        auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() / ("archivum_tests_" + std::to_string(stamp));
        std::filesystem::create_directories(path_);
    }

    ~ScopedTemp() {
        std::error_code ignored;
        if (path_.filename().string().starts_with("archivum_tests_")) {
            std::filesystem::remove_all(path_, ignored);
        }
    }

    const std::filesystem::path& path() const { return path_; }

   private:
    std::filesystem::path path_;
};

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void write_file(const std::filesystem::path& path, const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    require(out.good(), "failed to open " + path.string());
    out << content;
}

std::string quote(const std::filesystem::path& path) { return "\"" + path.string() + "\""; }

void run_command(const std::string& command) {
    int code = std::system(command.c_str());
    require(code == 0, "command failed: " + command);
}

archivum::Node make_node(archivum::NodeId id, const std::string& name, std::vector<std::string> references) {
    archivum::Node node;
    node.id = id;
    node.name = name;
    node.file_path = "unit.cpp";
    node.signature = name;
    node.references = std::move(references);
    node.start_line = static_cast<uint32_t>(id);
    node.end_line = static_cast<uint32_t>(id);
    node.type = archivum::NodeType::FUNCTION;
    node.interface_hash = id * 17;
    node.source_hash = id * 19;
    return node;
}

const archivum::Node* find_node(const std::vector<archivum::Node>& nodes, const std::string& name) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const archivum::Node& node) { return node.name == name; });
    return it == nodes.end() ? nullptr : &*it;
}

void parser_extracts_structural_metadata() {
    ScopedTemp temp;
    std::filesystem::path sample = temp.path() / "sample.cpp";
    write_file(sample,
               "namespace alpha {\n"
               "struct Payload { int count; };\n"
               "int helper() { return 1; }\n"
               "class Widget {\n"
               "public:\n"
               "    int value() const { return helper(); }\n"
               "};\n"
               "int outer() { Widget widget; return widget.value(); }\n"
               "}\n");

    archivum::ASTParser parser;
    std::vector<archivum::Node> nodes = parser.parse_file(sample.string());

    const archivum::Node* payload = find_node(nodes, "Payload");
    const archivum::Node* helper = find_node(nodes, "helper");
    const archivum::Node* widget = find_node(nodes, "Widget");
    const archivum::Node* value = find_node(nodes, "value");
    const archivum::Node* outer = find_node(nodes, "outer");

    require(payload != nullptr && payload->type == archivum::NodeType::STRUCT, "struct not extracted");
    require(helper != nullptr && helper->type == archivum::NodeType::FUNCTION, "function not extracted");
    require(widget != nullptr && widget->type == archivum::NodeType::CLASS, "class not extracted");
    require(value != nullptr && value->type == archivum::NodeType::METHOD, "method not extracted");
    require(outer != nullptr && outer->interface_hash != 0 && outer->source_hash != 0, "function hashes missing");
    require(std::find(outer->references.begin(), outer->references.end(), "Widget") != outer->references.end(),
            "references missing class use");

    std::vector<archivum::Node> second_pass = parser.parse_file(sample.string());
    const archivum::Node* second_outer = find_node(second_pass, "outer");
    require(second_outer != nullptr && second_outer->id == outer->id, "node id is not stable");
}

void graph_infers_transitive_impact() {
    archivum::DependencyGraph graph;
    graph.register_node(make_node(1, "Provider", {}));
    graph.register_node(make_node(2, "Consumer", {"Provider"}));
    graph.register_node(make_node(3, "Leaf", {"Consumer"}));
    graph.infer_dependencies();

    std::unordered_set<archivum::NodeId> radius = graph.calculate_blast_radius(1);
    require(radius.size() == 3, "transitive inferred impact is incomplete");
    require(graph.edge_count() == 2, "unexpected inferred edge count");
}

void graph_handles_cycles() {
    archivum::DependencyGraph graph;
    graph.register_node(make_node(1, "A", {}));
    graph.register_node(make_node(2, "B", {}));
    graph.register_node(make_node(3, "C", {}));
    graph.add_dependency(1, 2);
    graph.add_dependency(2, 3);
    graph.add_dependency(3, 1);

    std::unordered_set<archivum::NodeId> radius = graph.calculate_blast_radius(1);
    require(radius.size() == 3, "cycle traversal did not converge");
}

void git_scanner_reports_changed_ranges() {
    ScopedTemp temp;
    run_command("git init -q " + quote(temp.path()));
    run_command("git -C " + quote(temp.path()) + " config user.email archivum@example.invalid");
    run_command("git -C " + quote(temp.path()) + " config user.name Archivum");
    run_command("git -C " + quote(temp.path()) + " config commit.gpgsign false");
    run_command("git -C " + quote(temp.path()) + " config core.autocrlf false");

    write_file(temp.path() / "src" / "tool.cpp", "int answer() {\n    return 41;\n}\n");
    run_command("git -C " + quote(temp.path()) + " add .");
    run_command("git -C " + quote(temp.path()) + " commit -q -m initial");

    archivum::GitScanner first_scan(temp.path().string());
    std::string base = first_scan.resolve_reference("HEAD");

    write_file(temp.path() / "src" / "tool.cpp", "int answer() {\n    return 42;\n}\n");
    write_file(temp.path() / "include" / "tool.hpp", "int answer();\n");
    run_command("git -C " + quote(temp.path()) + " add .");
    run_command("git -C " + quote(temp.path()) + " commit -q -m update");

    archivum::GitScanner scanner(temp.path().string());
    std::string head = scanner.resolve_reference("HEAD");
    std::optional<std::string> parent = scanner.parent_of(head);
    std::vector<archivum::FileDiff> diffs = scanner.calculate_diff(base, head);

    require(parent.has_value() && parent.value() == base, "parent commit resolution failed");
    require(diffs.size() == 2, "diff did not include modified and added files");

    bool saw_source = false;
    bool saw_header = false;
    for (const archivum::FileDiff& diff : diffs) {
        saw_source = saw_source || diff.file_path == "src/tool.cpp";
        saw_header = saw_header || diff.file_path == "include/tool.hpp";
        require(!diff.modified_lines.empty(), "diff returned empty ranges");
        for (const archivum::ModifiedRange& range : diff.modified_lines) {
            require(range.start_line >= 1 && range.end_line >= range.start_line, "invalid changed range");
        }
    }

    require(saw_source && saw_header, "expected diff paths missing");
}

void config_loads_defaults_and_overrides() {
    ScopedTemp temp;
    std::filesystem::path config_path = temp.path() / ".archivum.yml";
    write_file(config_path,
               "docs_dir: reference\n"
               "model: gpt-test\n"
               "write_mode: none\n"
               "max_symbols: 7\n"
               "fail_on_provider_error: true\n");

    archivum::ArchivumConfig config = archivum::load_config(config_path);
    require(config.docs_dir == "reference", "docs_dir override failed");
    require(config.model == "gpt-test", "model override failed");
    require(archivum::write_mode_name(config.write_mode) == "none", "write mode override failed");
    require(config.max_symbols == 7, "max_symbols override failed");
    require(config.fail_on_provider_error, "provider error override failed");
}

void documentation_writer_creates_index_symbols_and_manifest() {
    ScopedTemp temp;
    write_file(temp.path() / "sample.cpp",
               "int provider() {\n"
               "    return 42;\n"
               "}\n"
               "int consumer() {\n"
               "    return provider();\n"
               "}\n");

    archivum::Node provider = make_node(1, "provider", {});
    provider.file_path = "sample.cpp";
    provider.signature = "int provider()";
    provider.start_line = 1;
    provider.end_line = 3;

    archivum::Node consumer = make_node(2, "consumer", {"provider"});
    consumer.file_path = "sample.cpp";
    consumer.signature = "int consumer()";
    consumer.start_line = 4;
    consumer.end_line = 6;

    archivum::ArchivumConfig config;
    config.docs_dir = "docs";
    config.write_mode = archivum::WriteMode::NONE;

    archivum::AnalysisReport report;
    report.base_sha = "base";
    report.head_sha = "head";
    report.source_file_count = 1;
    report.changed_file_count = 1;
    report.graph_node_count = 2;
    report.graph_edge_count = 1;
    report.mutated_nodes = {provider};
    report.context_nodes = {provider, consumer};

    archivum::DocumentationWriteResult first =
        archivum::write_documentation(config, report, "Provider now returns the documented value.", temp.path());
    archivum::DocumentationWriteResult second =
        archivum::write_documentation(config, report, "Provider now returns the documented value.", temp.path());

    require(first.changed, "documentation writer did not create files");
    require(!second.changed, "documentation writer should be idempotent");
    require(std::filesystem::exists(temp.path() / "docs" / "index.md"), "index was not created");
    require(std::filesystem::exists(temp.path() / "docs" / "archivum-manifest.json"), "manifest was not created");
    require(std::filesystem::exists(temp.path() / "docs" / "symbols"), "symbols folder was not created");
}

void provider_skips_when_key_is_absent() {
    archivum::ProviderRequest request;
    request.provider = "openai";
    request.endpoint = "https://api.openai.com/v1/responses";
    request.model = "gpt-test";
    request.api_key_env = "ARCHIVUM_TEST_MISSING_KEY";
    request.reasoning_effort = "low";
    request.verbosity = "low";
    request.prompt = "hello";
    request.fail_on_error = false;

    require(!archivum::generate_documentation_update(request).has_value(), "provider should skip without credentials");
}

void run_test(const std::string& name, const std::function<void()>& test) {
    test();
    std::cout << "[PASS] " << name << "\n";
}

}

int main() {
    archivum::init_git_subsystem();

    try {
        run_test("parser_extracts_structural_metadata", parser_extracts_structural_metadata);
        run_test("graph_infers_transitive_impact", graph_infers_transitive_impact);
        run_test("graph_handles_cycles", graph_handles_cycles);
        run_test("git_scanner_reports_changed_ranges", git_scanner_reports_changed_ranges);
        run_test("config_loads_defaults_and_overrides", config_loads_defaults_and_overrides);
        run_test("documentation_writer_creates_index_symbols_and_manifest",
                 documentation_writer_creates_index_symbols_and_manifest);
        run_test("provider_skips_when_key_is_absent", provider_skips_when_key_is_absent);
    } catch (const std::exception& error) {
        archivum::shutdown_git_subsystem();
        std::cerr << "[FAIL] " << error.what() << "\n";
        return 1;
    }

    archivum::shutdown_git_subsystem();
    return 0;
}
