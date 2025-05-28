#!/bin/bash

# Uruchom catcher w tle
./catcher &

# Pobierz PID procesu catcher
CATCHER_PID=$!

# Uruchom sender_test.sh
./test/sender_test.sh $CATCHER_PID

# Poczekaj na zakończenie catcher
wait $CATCHER_PID
echo "----------------------------------------"