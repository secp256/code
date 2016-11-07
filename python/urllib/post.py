#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import urllib2
import requests
import json

reload(sys)
sys.setdefaultencoding('utf-8') 

def main():
  url = "http://206.99.94.97:8780/verificationCodeFromInputStream"
  files = {'file': open('vcode.jpg', 'rf')}
  r = requests.post(url, files = files)
  js = json.loads(r.text)
  print js['verificationCode']

if __name__ == '__main__':
  main()
