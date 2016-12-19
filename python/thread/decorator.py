#!/usr/bin/env python
# -*- coding: utf-8 -*-

def f1(arg):
  print "f1"
  rl = arg()
  print rl
  return rl + "f1"

# @符作符相当于 f1(f2())
#这个运算其实在开始 import 时就已经在执行了，这也就是 f1 f2 f2r这几行会在 start 之前就出现的原因
@f1
def f2(arg = ""):
  print "f2"
  return arg + "f2r"
# 而当运行到代码时，其实 f2函数已经不存在了， f2 在这里已经变成一个字符串了
print "start"
print f2
