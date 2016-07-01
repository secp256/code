#!/bin/bash

trap "echo hello" 2 15
trap "exit 17" 3

while :
do
    sleep 1
done
