#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

def main():
  """
  find needle in haystack
  """
  if len(sys.argv) < 3:
    print 'usage: python %s string1 string2' % sys.argv[0] 
    sys.exit(1)

  haystack = sys.argv[1]
  needle = sys.argv[2]

  len_haystack = len(haystack)
  len_needle = len(needle)

  if len_needle > len_haystack:
    print -1
  elif len_needle == len_haystack:
    print 0 if haystack == needle else -1
  else:
    delta = len_haystack - len_needle + 1
    for i in range(0, delta):
      if haystack[i:i+len_needle] == needle:
        print i
        break
    else:
      print -1

if __name__ == '__main__':
  main()
