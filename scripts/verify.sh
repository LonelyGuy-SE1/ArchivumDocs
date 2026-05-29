#!/usr/bin/env bash
set -euo pipefail

build_dir="${1:-build}"

cmake -B "$build_dir" -S . -DCMAKE_BUILD_TYPE=Release
cmake --build "$build_dir" --config Release -j"$(nproc 2>/dev/null || echo 2)"
ctest --test-dir "$build_dir" --output-on-failure
ARCHIVUM_UPDATE_DOCS=false "$build_dir/archivum"
