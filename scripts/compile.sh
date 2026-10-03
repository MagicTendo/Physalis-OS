#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

set -e

echo "======== Standard errors ========" > ./scripts/logs/stderr.log

make clean
make compile 2>> ./scripts/logs/stderr.log
sudo make build_iso
bash ./scripts/tests.sh
make emulate 2>> ./scripts/logs/stderr.log