#!/usr/bin/env sh
set -e

mkdir -p build
cmake -S . -B build -DCMAKE_CXX_STANDARD=17 > log.txt 2>&1 || { echo "CMake configuration failed. See log.txt for details." >&2; exit 1; }
cmake --build build >> log.txt 2>&1 || { echo "Build failed. See log.txt for details." >&2; exit 1; }

