#!/bin/bash

# Build the program
echo "Building the program..."
make main

# Check if the build was successful
if [ $? -ne 0 ]; then
    echo "Build failed. Exiting."
    exit 1
fi

# Run the program
echo "Running the program..."
./main

# Check if the program ran successfully
if [ $? -ne 0 ]; then
    echo "Program execution failed."
    exit 1
fi

echo "Program executed successfully."