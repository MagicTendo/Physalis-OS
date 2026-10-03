#!/bin/bash

if xxd -b ./bin/loader.o | grep -q "01010101 10101010"; then
    echo "✅ Boot sector signature found!"
else
    echo "❌ No boot sector signature found..."
fi

if grub-file --is-x86-multiboot2 ./bin/kernel.bin; then
    echo "✅ ./bin/kernel.bin has multiboot2!"
else
    echo "❌ ./bin/kernel.bin doesn't support multiboot2..."
fi