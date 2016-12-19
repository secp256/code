#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from threading import Thread

value = 0

def getlock():
  global value
  new = value + 1
  time.sleep(0.001)
  value = new

threads = []

for i in range(200):
  t = Thread(target = getlock)
  t.start()
  threads.append(t)

for t in threads:
  t.join()

print value
