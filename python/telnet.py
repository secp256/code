#!/usr/bin/python

import socket

HOST='127.0.0.1'
PORT=4000

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))

while 1:
       cmd=raw_input("cmd:")
       s.sendall(cmd + "\n")
       data=s.recv(10240)
       if (len(data) > 0):
         print data
s.close()
