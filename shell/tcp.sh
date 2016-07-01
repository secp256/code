#!/bin/bash

ipaddr=127.0.0.1
port=999

if ! exec 5<> /dev/tcp/$ipaddr/$port
then
    exit 1
fi

echo -e "GET / HTTP/1.0\n" >&5

cat <&5
