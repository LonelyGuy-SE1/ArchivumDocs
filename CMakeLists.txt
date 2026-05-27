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
set(USE_SSH OFF CACHE BOOL "" FORCE) 
set(USE_HTTPS OFF CACHE BOOL "" FORCE) 
FetchContent_MakeAvailable(libgit2)

FetchContent_Declare(
  tree_sitter
  GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
  GIT_TAG        v0.22.6
)
FetchContent_MakeAvailable(tree_sitter)

FetchContent_Declare(
  tree_sitter_cpp
  GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter-cpp.git
  GIT_TAG        v0.22.0
)
FetchContent_MakeAvailable(tree_sitter_cpp)

add_executable(archivum 
    src/main.cpp 
    src/graph.cpp 
    src/parser.cpp 
    src/git_utils.cpp 
    src/api.cpp
    ${tree_sitter_SOURCE_DIR}/lib/src/lib.c
    ${tree_sitter_cpp_SOURCE_DIR}/src/parser.c
    ${tree_sitter_cpp_SOURCE_DIR}/src/scanner.c
)

target_include_directories(archivum PRIVATE 
    include
    ${tree_sitter_SOURCE_DIR}/lib/include
    ${tree_sitter_cpp_SOURCE_DIR}/src
    ${libgit2_SOURCE_DIR}/include
)

if (TARGET libgit2package)
    set(GIT2_TARGET libgit2package)
elseif (TARGET libgit2::libgit2)
    set(GIT2_TARGET libgit2::libgit2)
elseif (TARGET libgit2)
    set(GIT2_TARGET libgit2)
elseif (TARGET git2)
    set(GIT2_TARGET git2)
else()
    message(FATAL_ERROR "[ArchivumDocs] CRITICAL: libgit2 target name not found in the build graph.")
endif()

target_link_libraries(archivum PRIVATE ${GIT2_TARGET})
