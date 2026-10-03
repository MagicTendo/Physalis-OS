#!/bin/bash

set -e

volume="/dev/sda"

if [ ! -z $1 ]; then
    volume=$1
fi

if [ $(df | grep $volume | wc -l) -eq 1 ]; then
    df | grep $volume

    read -p "Proceeds with this volume ? (y/N): " action

    if [ "$action" == "y" ]; then
        sudo dd if=./builds/physalis-os.iso of="${volume}"
        sync
        umount ${volume}

        echo "Enjoy Physalis OS on ${volume}!"
    fi
else
    echo "The specified volume seems to not exist..."
fi