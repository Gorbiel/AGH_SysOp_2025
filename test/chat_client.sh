#!/bin/bash
DONE_FILE="$1"
SERVER_PID="$2"

trap "touch $DONE_FILE; exit" SIGINT SIGTERM EXIT

./chat_client "$SERVER_PID"