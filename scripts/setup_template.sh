#!/usr/bin/env bash

set -eu

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 template_name"
    exit 1
fi

CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_DIR=$CURRENT_DIR/../dev
TEMPLATE_DIR=$CURRENT_DIR/../templates
TEMPLATE_NAME=$1

mkdir -p $TARGET_DIR
cp $TEMPLATE_DIR/CMakeLists.txt $TARGET_DIR
cp $TEMPLATE_DIR/$TEMPLATE_NAME/* $TARGET_DIR

echo "Installed template $TEMPLATE_NAME"
