#!/bin/bash

echo "Testing doctor.c"
echo

# Lista testów: ("liczba_pacjentów liczba_farmaceutów" ...)
tests=(
    "6 2"
    "9 3"
    "3 1"
    "12 4"
    "9 2"
)

total_tests=${#tests[@]}
test_num=1

for test in "${tests[@]}"; do
    set -- $test
    echo "Test $test_num/$total_tests: $1 pacjentów, $2 farmaceutów"
    ./doctor "$1" "$2"
    echo
    ((test_num++))
done

echo "Doctor finished"
echo "----------------------------------------"