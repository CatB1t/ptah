#!/usr/bin/env bash
# Format all C/C++ sources in include/ and src/ with clang-format.
set -euo pipefail

cd "$(dirname "$0")/.."

find include src \
    -type f \
    \( -name '*.h' -o -name '*.hpp' -o -name '*.inl' \
    -o -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \) \
    -print0 | xargs -0 clang-format -i --style=file

echo "Formatting done."
