#!/bin/bash

# Uruchom server w tle
./chat_server &

# Pobierz PID serwera
SERVER_PID=$!

# Uruchom klientów
./test/chat_client.sh $SERVER_PID &
CLIENT_1_PID=$!

./test/chat_client.sh $SERVER_PID &
CLIENT_2_PID=$!

# Poczekaj na zakończenie obu klientów
wait $CLIENT_1_PID
wait $CLIENT_2_PID

# Zakończ działanie serwera
kill $SERVER_PID
echo "----------------------------------------"