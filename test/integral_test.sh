#!/bin/bash

echo "Testing integral.c"
./integral
echo

echo "Running: ./integral 0.01 2"
./integral 0.01 2
echo

echo "Running: ./integral 0.001 4"
./integral 0.001 4
echo

echo "Running: ./integral 0.0001 8"
./integral 0.0001 8
echo

echo "Running: ./integral 0.1 1"
./integral 0.1 1
echo

echo "Running: ./integral 0.05 3"
./integral 0.05 3
echo

echo "Running: ./integral 0.001 10"
./integral 0.001 10
echo

echo "Running: ./integral 0.00001 16"
./integral 0.00001 16
echo

echo "Running: ./integral 0.0001 5"
./integral 0.0001 5
echo

echo "Running: ./integral 0.00001 20"
./integral 0.00001 20
echo

echo "Running: ./integral 0.000001 32"
./integral 0.000001 32
echo


echo "Integral test finished"
echo "----------------------------------------"