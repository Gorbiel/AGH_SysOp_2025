#!/bin/bash

# List of example arguments for pipe_1
ARGUMENTS_LIST=(
    "0.01 1"       # Small rectangle width, single process
    "0.1 4"        # Larger rectangle width, multiple processes
    "0.001 8"      # Very small rectangle width, many processes
    "0.05 2"       # Medium rectangle width, two processes
    "0.01 16"      # Small rectangle width, high number of processes
    "0.1 32"       # Larger rectangle width, very high number of processes
    "0.0001 64"    # Extremely small rectangle width, maximum processes
    "0.05 0"       # Medium rectangle width, invalid process count (edge case)
    "0.05 -1"      # Medium rectangle width, negative process count (edge case)
    "0 4"          # Zero rectangle width (invalid case)
)

# Iterate over the argument list and run pipe_1
for ARGS in "${ARGUMENTS_LIST[@]}"; do
    echo "Running pipe_1 with arguments: $ARGS"
    ./pipe_1 $ARGS
    echo "----------------------------------------"
done