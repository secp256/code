#!/bin/bash

# nc -k -l 1567

num=5000
host=192.168.5.94
port=1567

for((i = 0;i < $num; ++i)) do
    nc $host $port < /dev/null
    if [ $? -ne "0" ] ; then
        echo "*"
    fi
done
