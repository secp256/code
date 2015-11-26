#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import urllib2

reload(sys)
sys.setdefaultencoding('utf-8') 

def download_page(url, proxy = None, referer = None, data = None):
  # set http proxy
  if proxy:
    handlers = [urllib2.ProxyHandler({'http': 'http://%s/' % proxy})]
    opener = urllib2.build_opener(*handlers)
  else:
    # null_proxy_handler = urllib2.ProxyHandler({})
    # opener = urllib2.build_opener(*null_proxy_handler)
    opener = urllib2.build_opener()

  request = urllib2.Request(url, data)
  # request.get_method = lambda: 'PUT' # or 'DELETE'

  # set referer
  if referer:
    request.add_header('Referer', referer)

  # add header
  request.add_header('User-Agent', 'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36')
  request.add_header('Accept-Language', 'en-US,en;q=0.5')

  response = opener.open(request, timeout=100)
  #response = urllib2.urlopen(request, timeout = 100).read()
  # redirected_url = response.geturl()
  page_buf = response.read()
  return page_buf
