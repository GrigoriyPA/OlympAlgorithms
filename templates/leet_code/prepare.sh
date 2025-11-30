#!/usr/bin/env bash

# Expected to be placed in dev/ directory

set -eu

if [ "$#" -ne 0 ]; then
    echo "Usage: $0"
    exit 1
fi

CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

$CURRENT_DIR/../scripts/expand_includes.sh solution.h submit.h
