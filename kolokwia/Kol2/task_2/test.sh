#!/bin/bash

# Test the programs
echo "Testing shm_write..."
./shm_write
if [ $? -ne 0 ]; then
    echo "Failed to execute shm_write"
    exit 1
fi

echo "Testing shm_read..."
output=$(./shm_read)
if [ $? -ne 0 ]; then
    echo "Failed to execute shm_read"
    exit 1
fi

echo "Output from shm_read: $output"

# Verify the output
if [ "$output" == "Hello" ]; then
    echo "Test passed: shm_read successfully read the data written by shm_write."
else
    echo "Test failed: shm_read did not read the expected data."
    exit 1
fi

echo "Build and test completed successfully."