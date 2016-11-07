#!/usr/bin/env python
# -*- coding: utf-8 -*-

from multiprocessing import Pool
from time import sleep

def f(x):
  for i in range(5):
    print '%s --- %s ' % (x, i)
    sleep(0.5)

def main():
  pool = Pool(processes=5)    # set the processes max number 3
  for i in range(10,30):
    result = pool.apply_async(f, (i,))
  pool.close()
  pool.join()
  if result.successful():
    print 'successful'

if __name__ == "__main__":
  main()
