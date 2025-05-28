#!/bin/bash

# List of example arguments for pipe_2_IO
ARGUMENTS_LIST=(
    "0 1 0.01"       # Basic range
    "0 10 0.1"       # Larger range
    "-5 5 0.05"      # Negative start
    "0 0.1 0.001"    # Small range
    "1.5 3.5 0.05"   # Fractional range
    "0 100 0.001"    # Large range with small width
    "0 1 0.0001"     # Very small width
)

# Iterate over the argument list and run pipe_2_IO and pipe_2_calculator
for ARGS in "${ARGUMENTS_LIST[@]}"; do
    echo "Running pipe_2_IO with arguments: $ARGS"
    
    # Start pipe_2_IO in the background
    ./pipe_2_IO $ARGS &
    IO_PID=$!

    # Wait briefly to ensure pipe_2_IO sets up the named pipes
    sleep 1

    # Start pipe_2_calculator in the background
    echo "Starting pipe_2_calculator..."
    ./pipe_2_calculator &
    CALCULATOR_PID=$!

    # Wait for both processes to finish
    wait $IO_PID
    wait $CALCULATOR_PID

    echo "Finished processing with arguments: $ARGS"
    echo "----------------------------------------"
done