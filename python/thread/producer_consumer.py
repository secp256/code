#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import threading
import Queue

class Consumer(threading.Thread):
  def __init__(self, queue):
    threading.Thread.__init__(self)
    self._queue = queue

  def run(self):
    while True:
      # queue.get() blocks thre current thread util and item is retrived
      msg = self._queue.get()
      if isinstance(msg, str) and msg == 'quit':
        break
      print "I'm a thread, and i received %s !" % msg
    print 'Bye bye!'


def Producer():
  # Queue is used to share items between the threads
  queue = Queue.Queue()

  # create an instance of the worker
  worker = Consumer(queue)
  # start calls the internal run() method to kick off the thread
  worker.start()

  start_time = time.time()
  while time.time() - start_time < 5:
    queue.put('somethine at %s' % time.time())
    time.sleep(1)


  queue.put('quit')
  # wait for the thread to close down
  worker.join()

if __name__ == '__main__':
  Producer()
