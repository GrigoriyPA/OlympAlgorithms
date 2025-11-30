#!/usr/bin/env bash

set -eu

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 source_file target_file"
    exit 1
fi

CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEV_DIR=$CURRENT_DIR/../dev
SOURCE_FILE=$DEV_DIR/$1
TARGET_FILE=$DEV_DIR/$2

(cat $CURRENT_DIR/../libs/system_includes.inc && gcc -E -fdirectives-only -undef -P -D __ALG_DISABLE_SYSTEM_INCLUDES -nostdinc -I $CURRENT_DIR/.. $SOURCE_FILE | sed '/^#define __/d') > $TARGET_FILE
xclip -selection clipboard < $TARGET_FILE

echo "Result saved into file: $(realpath $TARGET_FILE) and copied to clipboard"
