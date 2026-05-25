cmake_minimum_required(VERSION 3.20)
project(ArchivumDocs VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  libgit2
  GIT_REPOSITORY https://github.com/libgit2/libgit2.git
  GIT_TAG        v1.8.1
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_CLI OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(libgit2)

FetchContent_Declare(
  tree_sitter
  GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
  GIT_TAG        v0.22.6
)
FetchContent_Populate(tree_sitter)

add_library(tree-sitter STATIC ${tree_sitter_SOURCE_DIR}/lib/src/lib.c)
target_include_directories(tree-sitter PUBLIC ${tree_sitter_SOURCE_DIR}/lib/include)

add_executable(archivum 
    src/main.cpp 
    src/graph.cpp 
    src/parser.cpp 
    src/git_utils.cpp 
    src/api.cpp
)

target_include_directories(archivum PRIVATE include)

target_link_libraries(archivum PRIVATE libgit2 tree-sitter)