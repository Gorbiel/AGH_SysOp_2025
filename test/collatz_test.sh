#!/bin/bash

echo "Testing static collatz library"
# Execute countdown from the parent directory
./client_static
echo "Static collatz finished"
echo "----------------------------------------"
echo "Testing dynamic collatz library"
# Execute countdown from the parent directory
./client_dynamic
echo "Dynamic collatz finished"
echo "----------------------------------------"
echo "Testing shared collatz library"
# Execute countdown from the parent directory
./client_shared
echo "Shared collatz finished"
echo "----------------------------------------"