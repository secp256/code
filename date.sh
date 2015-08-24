#!/bin/bash

UPDATE_INTERVAL=600

now=`date +%s`
echo "current time: $now"

last_change_time=`stat -c %Z c.c`
echo "last change time: $last_change_time"

interval=$((now-last_change_time))
echo "interval: $interval"
