#!/bin/bash

# Check if PID is provided as an argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <PID>"
    exit 1
fi

PID=$1

# Execute ./sender <PID> 1 three times
echo "[SENDER]  Executing ./sender $PID 1 three times"
for i in {1..3}; do
    ./sender $PID 1
    sleep 1
done



# Execute ./sender <PID> 2
echo "[SENDER]  Executing ./sender $PID 2, please wait..."
./sender $PID 2

# Wait for 10 seconds
sleep 10

# Execute ./sender <PID> 3
echo "[SENDER]  Executing ./sender $PID 3"
./sender $PID 3

# Send SIGINT to the catcher
for i in {1..3}; do
    kill -SIGINT $PID
    sleep 1
done


# Execute ./sender <PID> 4
echo "[SENDER]  Executing ./sender $PID 4"
./sender $PID 4

# Send SIGINT to the catcher
for i in {1..3}; do
    kill -SIGINT $PID
    sleep 1
done

# Execute ./sender <PID> 5
echo "[SENDER]  Executing ./sender $PID 5"
./sender $PID 5