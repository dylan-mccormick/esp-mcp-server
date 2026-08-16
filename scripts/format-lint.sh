#!/usr/bin/env bash
set -euo pipefail

echo "Running clang-format..."
find src \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} \;

echo "Running PIO Linter..."
pio check