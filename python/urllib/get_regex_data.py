#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re

def get_regex_data(regex, buf):
  group = re.search(regex, buf)
  if group:
    return group.groups()[0]
  else:
    return ''

