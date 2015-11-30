#!/usr/bin/env python
# -*- coding: utf-8 -*-

import timeit

def test_range(n):
  sum = 0
  for i in range(n):
    sum += i
  return sum

def test_xrange(n):
  sum = 0
  for i in xrange(n):
    sum += i
  return sum

t_range = timeit.Timer("test_range(100)", "from __main__ import test_range");
t_xrange = timeit.Timer("test_xrange(100)", "from __main__ import test_xrange");

print(t_range.timeit())
print(t_xrange.timeit())

print(t_range.repeat(3, 10000))
print(t_xrange.repeat(3, 10000))
