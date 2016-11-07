#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import re
import urllib2
import random

base = [str(x) for x in range(10)] + [ chr(x) for x in range(ord('A'),ord('A')+6)]

def dec2hex(string_num):
  num = int(string_num)
  mid = []
  while True:
    if num == 0: break
    num,rem = divmod(num, 16)
    mid.append(base[rem])

  return ''.join([str(x) for x in mid[::-1]])

def download_page(url, data = None, headers = {}):
  request = urllib2.Request(url, data)

  # set headers
  for key, value in headers.items():
    request.add_header(key, value)

  opener = urllib2.build_opener()
  response = opener.open(request, timeout=100)
  content = response.read()
  return content

def get_regex_data(regex, buf):
  group = re.search(regex, buf)
  if group:
    return group.groups()[0]
  else:
    return ''

def main():
  video_id = 446008
  url = 'http://video.i.ua/video/%d' % video_id

  content = download_page(url)
  key= get_regex_data('&key=(\d+)&', content)
  key_time= get_regex_data('&keyTime=(\d+)&', content)

  download_url = 'http://vs1.i.ua/v/%s/2/%s/%s/%d.mp4' % (dec2hex(video_id), dec2hex(key_time), dec2hex(key), random.randint(1,10000000))
  print download_url
  return

if __name__ == '__main__':
  main()
