#!/usr/bin/python

import socket

HOST='localhost'
PORT=9002

msg='xxxxx'
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))

def test():
  s.sendall(msg + "\n")
  print "send msg finished, length: [%d]" % len(msg)
  data = s.recv(10240)
  print "receive data finished, length: [%d]" % len(data)
  if (len(data) > 0):
    print 'response msg: [%s]' % data
  else:
    print "timeout, no data received"
  s.close()

if __name__ == '__main__':
  test()
