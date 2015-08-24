#!/usr/bin/python

import threading
from time import ctime, sleep

def music():
  for i in range(2):
    print 'I was at music. %s' % (ctime())
    sleep(1)

def movie():
  for i in range(2):
    print 'I was at movie. %s' % (ctime())
    sleep(3)

threads = []
t1 = threading.Thread(target=music)
threads.append(t1)
t2 = threading.Thread(target=movie)
threads.append(t2)

if __name__ == '__main__':
  for t in threads:
    t.setDaemon(True)
    t.start()

  for t in threads:
    threading.Thread.join(t)
  print 'all over. %s' % ctime()
