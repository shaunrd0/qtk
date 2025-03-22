#!/usr/bin/env bash
# Helper script to run clang-tidy and clang-format.
# This should be executed from the root of the repository:
#
# git clone https://git.shaunreed.com/shaunrd0/qtk
# cd qtk
# ./tools/format.sh

# Configure and build qtk
cmake -B build && cmake --build build -- -j $(nproc --ignore=1)

# Run clang-tidy and clang-format
SOURCES="src/**/*.cpp src/**/*.h example-app/*.cpp example-app/*.h"
run-clang-tidy \
    -p build/ -j $(nproc --ignore=1) -fix -config-file=.clang-tidy $SOURCES
clang-format -i --style=file:.clang-format $SOURCES
