#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

def main():
  """
  从输入的文件中读取字符串，先去重，后排序，最后输出结果
  """
  if len(sys.argv) < 2:
    print 'usage: python %s filename' % sys.argv[0]
    sys.exit(1)

  file_name = sys.argv[1]
  with open(file_name) as f:
    line = f.read().strip()
    print 'origin:', line
    s = set(line) # 用set去重
    l = list(s)   # set --> list
    l.sort()      #  排序
    result = "".join(l) # list转成字符串
    print 'sorted:', result

if __name__ == '__main__':
  main()
