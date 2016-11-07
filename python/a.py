#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

def main():
  en_file = sys.argv[1]
  a =  ["J", "B", "m", "X", "0", "z", "4", "T", "9", "g", "k", "M", "G", "R", "y", "7", "l", "8", "s", "U", "H", "f", "u", "2", "Q", "="]
  b =  ["N", "3", "w", "x", "D", "v", "V", "d", "I", "b", "o", "p", "1", "c", "5", "e", "i", "Y", "Z", "a", "W", "L", "6", "t", "n", "q"]

  for i in b:
    en_file = en_file.replace(i, a[b.index(i)])
  print en_file
  return 

if __name__ == '__main__':
  main()
