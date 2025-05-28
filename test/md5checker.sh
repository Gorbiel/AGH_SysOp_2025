#!/bin/bash

ART_DIR="resources/art"
RESTORED_DIR="restored"

# Check if the directories exist
if [ ! -d "$ART_DIR" ]; then
    echo "Directory $ART_DIR does not exist."
    exit 1
fi

if [ ! -d "$RESTORED_DIR" ]; then
    echo "Directory $RESTORED_DIR does not exist."
    exit 1
fi

# Iterate over each file in the art directory
for file in "$ART_DIR"/*; do
    if [ -f "$file" ]; then
        base_filename=$(basename "$file")
        restored_file="$RESTORED_DIR/${base_filename}"

        if [ -f "$restored_file" ]; then
            echo "Checking $base_filename"
            # Calculate MD5 sums
            md5_art=$(md5sum "$file" | awk '{ print $1 }')
            md5_reversed=$(md5sum "$restored_file" | awk '{ print $1 }')

            # Print MD5 sums
            echo "MD5 sum for original: $md5_art"
            echo "MD5 sum for reversed: $md5_reversed"

            # Compare MD5 sums
            if [ "$md5_art" == "$md5_reversed" ]; then
                echo "MD5 match for $base_filename"
            else
                echo "MD5 mismatch for $base_filename"
            fi
        else
            echo "Restored file $restored_file does not exist."
        fi
    fi

    echo ""

done

exit 0