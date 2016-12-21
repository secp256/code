#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import threading
from random import random
from Queue import Queue

def double(n):
  return n * 2

class Worker(threading.Thread):
  def __init__(self, queue):
    super(Worker, self).__init__()
    self._q = queue
    self.daemon = True
    self.start()

  def run(self):
    while True:
      f, args, kwargs = self._q.get()
      try:
        result = f(*args, **kwargs)
        print 'USE: {}, result {}'.format(self.name, result)
      except Exception as e:
        print e
      self._q.task_done()

class ThreadPool(object):
  def __init__(self, num_t = 5):
    self._q = Queue(num_t)
    for i in range(num_t):
      Worker(self._q)

  def add_task(self, f, *args, **kwargs):
    self._q.put((f, args, kwargs))

  def wait_complete(self):
    self._q.join()

pool = ThreadPool()
for i in range(17):
  wt = random()
  pool.add_task(double, wt)
  time.sleep(wt)
pool.wait_complete()
