#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <SERVER_PID>"
  exit 1
fi

SERVER_PID=$1

# Open chat_client in new terminal
gnome-terminal -- ./chat_client $SERVER_PID &

# Wait a moment to initialize
sleep 1

# Simulate writing and sending a message
xdotool type "Hello from client_1!"
xdotool key Return

sleep 1
exit 0