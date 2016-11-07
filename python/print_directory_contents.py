#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os

def print_directory_contents(path):
  for child_path in os.listdir(path):
    t_child_path = os.path.join(path, child_path)
    if os.path.isdir(t_child_path):
      print_directory_contents(t_child_path)
    else:
      print t_child_path
  return

def main():
  print_directory_contents("/home/zhuliting/test")

if __name__ == '__main__':
  main()
