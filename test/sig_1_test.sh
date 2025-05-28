#!/bin/bash

# Run ./sig_1 with different arguments and send SIGUSR1 to its PID
for mode in none ignore handler mask; do
    echo "Running ./sig_1 with mode: $mode"
    
    # Start ./sig_1 in the background
    ./sig_1 $mode
    echo "----------------------------------------"
done