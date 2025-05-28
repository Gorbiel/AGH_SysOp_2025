#!/bin/bash

./chat_server &
SERVER_PID=$!

CLIENT1_DONE=client1.done
CLIENT2_DONE=client2.done
rm -f $CLIENT1_DONE $CLIENT2_DONE

gnome-terminal -- bash -c "./test/chat_client.sh $CLIENT1_DONE $SERVER_PID; exec bash" &
gnome-terminal -- bash -c "./test/chat_client.sh $CLIENT2_DONE $SERVER_PID; exec bash" &

while [ ! -f $CLIENT1_DONE ] || [ ! -f $CLIENT2_DONE ]; do
    sleep 1
done

kill $SERVER_PID
echo "----------------------------------------"