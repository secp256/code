#!/usr/bin/env python
# coding:utf-8

import lxml.html
import urlparse
import urllib2
import optparse


IGNORE_EXT = ('swf', 'rar', 'zip', 'tar', 'xml') 

def download_page(url, proxy = None, data = None, headers = {}):
  # set http proxy
  if proxy:
    handler = urllib2.ProxyHandler({'http': 'http://%s/' % proxy})
    opener = urllib2.build_opener(handler)
  else:
    # null_proxy_handler = urllib2.ProxyHandler({})
    # opener = urllib2.build_opener(*null_proxy_handler)
    opener = urllib2.build_opener()

  request = urllib2.Request(url, data)
  # request.get_method = lambda: 'PUT' # or 'DELETE'

  # set headers
  for key, value in headers.items():
    request.add_header(key, value)

  response = opener.open(request, timeout=100)
  # in python source code, urlopen call opener.open
  # redirected_url = response.geturl()
  page_buf = response.read()
  return page_buf

def timestamp():
  return str(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))

def crawler(html, url):
  tmp = lxml.html.document_fromstring(html)
  tmp.make_links_absolute(url)
  links = tmp.iterlinks()
  link_list = []
  link_list = list(set([i[2] for i in links]))

  #过滤不期待页面后缀
  temp_list = []
  for i in link_list:
    if urlparse.urlparse(i)[2].split('.')[-1].lower() not in IGNORE_EXT:
      temp_list.append(i)
      link_list = temp_list

  return temp_list

def main():
  usage = "-u url"
  parser = optparse.OptionParser(usage=usage)
  parser.add_option("-u", "--url",
      dest = "url",
      default = "http://www.baidu.com",
      help="start the domain name"
      )
  (options, args) = parser.parse_args()

  url = options.url
  print url
  page_buf = download_page(url)
  url_list = crawler(page_buf, url)
  for i in url_list:
    print i

if __name__ == '__main__':
  main()
