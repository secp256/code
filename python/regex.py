#!/usr/bin/env python
# -*- coding: utf-8 -*-
import re

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

