#!/usr/bin/env python
# -*- coding: utf-8 -*-

def binarySearch(l, t):
  print t
  for i in l: print i,
  print
  low, high = 0, len(l) - 1
  while low < high:
    mid = (low + high) / 2
    print low, mid, high
    print l[low], l[mid], l[high]
    if l[mid] > t:
      high = mid
    elif l[mid] < t:
      low = mid + 1
    else:
      return "index: ", mid
  return False

if __name__ == '__main__':
  l = [1, 4, 12, 45, 66, 99, 120, 444]
  print binarySearch(l, 99)
  print binarySearch(l, 1)
  print binarySearch(l, 500)
