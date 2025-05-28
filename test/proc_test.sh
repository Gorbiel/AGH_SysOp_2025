#!/bin/bash

# List of example arguments for proc_1
ARGUMENTS_LIST_PROC_1=(
    "2"
    "4"
    "5"
)

# List of example arguments for proc_2
ARGUMENTS_LIST_PROC_2=(
    "."
    "./test"
    "./src"
)

# Iterate over the argument list and run pipe_1
for ARGS in "${ARGUMENTS_LIST_PROC_1[@]}"; do
    echo "Running proc_1.c with arguments: $ARGS"
    ./proc_1 $ARGS
    echo "----------------------------------------"
done

# Iterate over the argument list and run proc_2
for ARGS in "${ARGUMENTS_LIST_PROC_2[@]}"; do
    echo "Running proc_2.c with arguments: $ARGS"
    ./proc_2 $ARGS
    echo "----------------------------------------"
done
