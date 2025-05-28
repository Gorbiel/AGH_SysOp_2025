#!/bin/bash

echo "Testing socket chat (UDP)"

# Uruchom serwer w tle
echo "Starting server..."
./socket_chat_server_2 127.0.0.1 12345 > server.log 2>&1 &
SERVER_PID=$!
sleep 1

# Uruchom klienta Anna i wyślij polecenia
echo "Starting client Anna..."
(
  sleep 1
  echo "LIST"
  sleep 1
  echo "2ALL Hello from Anna!"
  sleep 1
  echo "2ONE Bob Hi Bob!"
  sleep 1
  echo "STOP"
) | ./socket_chat_client_2 Anna 127.0.0.1 12345 > anna.log 2>&1 &

# Uruchom klienta Bob i wyślij polecenia
echo "Starting client Bob..."
(
  sleep 2
  echo "LIST"
  sleep 2
  echo "STOP"
) | ./socket_chat_client_2 Bob 127.0.0.1 12345 > bob.log 2>&1 &

# Poczekaj na zakończenie klientów
sleep 8
echo "Clients finished"

# Zakończ serwer
kill $SERVER_PID
echo "Server stopped"

# Wyświetl logi
echo
echo "=== SERVER LOG ==="
cat server.log
echo "=== ANNA LOG ==="
cat anna.log
echo "=== BOB LOG ==="
cat bob.log
echo "=== END OF LOGS ==="
echo
echo "Socket chat test (UDP) finished"
echo "----------------------------------------"