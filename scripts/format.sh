#!/usr/bin/env bash
# Format all C/C++ sources in include/ and src/ with clang-format.
# Prints each file that was changed and a total count.
set -euo pipefail

cd "$(dirname "$0")/.."

changed=0
while IFS= read -r -d '' f; do
    if ! clang-format --dry-run -Werror --style=file "$f" >/dev/null 2>&1; then
        clang-format -i --style=file "$f"
        echo "  changed: $f"
        changed=$((changed + 1))
    fi
done < <(find include src \
    -type f \
    \( -name '*.h' -o -name '*.hpp' -o -name '*.inl' \
    -o -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \) \
    -print0)

echo "Formatting done: $changed file(s) changed."
