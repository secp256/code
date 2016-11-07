#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib
import re
from bs4 import BeautifulSoup
import re
import datetime
import random

random.seed(datetime.datetime.now())
def getlinks(articleUrl):
  html = urllib.urlopen('http://en.wikipedia.org' + articleUrl)
  bsObj = BeautifulSoup(html)
  return  bsObj.find('div', {'id':'bodyContent'})
