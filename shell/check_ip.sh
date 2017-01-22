#!/bin/sh

IP=$1
echo $IP|awk -F . '$1<=255&&$2<=255&&$3<=255&&$4<=255 {print "yes"}'
