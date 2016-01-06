#!/usr/bin/env python
# -*- coding: utf-8 -*-

from SocketServer import (TCPServer as TCP,
    StreamRequestHandler as SRH)
from time import ctime

HOST = ''
PORT = 9002
ADDR = (HOST, PORT)

class MyRequestHandler(SRH):
  def handle(self):
    print '...connected from:', self.client_address
    # data = self.rfile.readline()
    data = self.request.recv(10000)
    print 'received data from client, total length: [%d]' % len(data)

    msg = 'Success! I have received [%s] bytes!'  % len(data)
    # self.wfile.write(msg)
    self.request.send(msg)
    print 'response msg: [%s]' % msg

tcpServ = TCP(ADDR, MyRequestHandler)
print 'waiting for connection...'
tcpServ.serve_forever()
