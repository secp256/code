#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re

def get_regex_data(regex, data):
  pattern = re.search(regex, data)
  if pattern:
    return pattern.group(1)
  else:
    return ''

def test_1():
  strings = [ "<pypix>",
      "<foo",
      "bar>",
      "hello" ]

  for s in strings:
    pattern = re.search(r'^(<)?[a-z]+(?(1)>)$', s)
    # 1 表示分组 (<)，当然也可以为空因为后面跟着一个问号。当且仅当条件成立时它才匹配关闭的尖括号>
    # (?(?=regex)then|else)
    if pattern:
      print 'True'
    else:
      print 'False'

def test_2():
  ## 给分组命名
  string = "Hello foobar"
  pattern = re.search(r'(?P<hi>H.*)(?P<fstar>f.*)(?P<bstar>b.*)', string)
  print "f* => {0}".format(pattern.group('fstar')) # prints f* => foo          
  print "b* => {0}".format(pattern.group('bstar')) # prints b* => bar          
  print "h* => {0}".format(pattern.group('hi')) # prints b* => Hello

def test_3():
  return

if __name__ == '__main__':
  test_1()
  test_2()
