#!/bin/bash


for i in $(seq 10000)
do
    echo $i
    curl 'https://www.google.com/webmasters/tools/dmca-dashboard?hl=zh-CN&pid=2&authuser=1' -H 'User-Agent:Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36' -L -b google_cookie -o ${i}.html
    grep 'login' ${i}.html | wc -l
    sleep_second=$(($RANDOM%5+1))
    sleep $sleep_second
done
