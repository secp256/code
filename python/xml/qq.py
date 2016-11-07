#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import urllib2
import xml.etree.ElementTree as ET
import re
import libxml2

reload(sys)
sys.setdefaultencoding('utf-8') 

def get_regex_data(regex, buf):
  group = re.search(regex, buf)
  if group:
    return group.groups()[0]
  else:
    return ''

def get_charset(html):
  pattern = r'''<meta\b[^>]*\bcharset\s*=\s*('|")?(?P<charset>[^'">\s]+)'''
  charset_reg = re.compile(pattern, re.S)
  m = charset_reg.search(html)
  return m.group('charset') if m is not None else None

def get_html_doc(html):
  if html is None:
    return
  if len(html) == 0:
    return
  pattern = '[\\x00-\\x08\\x0b-\\x0c\\x0e-\\x1f]'
  html = re.sub(pattern, ' ', html)
  # get encoding
  encoding = get_charset(html)
  if encoding is None:
    encoding = 'utf-8'

  options = libxml2.HTML_PARSE_RECOVER | libxml2.HTML_PARSE_NOERROR \
          | libxml2.HTML_PARSE_PEDANTIC | libxml2.HTML_PARSE_NONET \
          | libxml2.HTML_PARSE_NOWARNING
  doc = libxml2.htmlReadMemory(html, len(html), None, encoding, options)
  return doc

def download_page(url, data = None, headers = {}):
  request = urllib2.Request(url, data)

  for key, value in headers.items():
    request.add_header(key, value)

  opener = urllib2.build_opener()
  response = opener.open(request, timeout=100)
  page_buf = response.read()
  return page_buf

def main():
  url = 'http://www.360doc.cn/article/3899926_504563407.html'
  content = download_page(url, None, {})
  content = str(content.encode('utf-8'))

  doc = get_html_doc(content)
  context = doc.xpathNewContext()
  nodes = context.xpathEval('//*[@class="article"]/a')

  for node in nodes:
    context.setContextNode(node)
    url_node = context.xpathEval("./@href")
    title_node = context.xpathEval("./text()")
    page_url = url_node[0].getContent()
    title = title_node[0].getContent()
    url_content = download_page(page_url)
    vid = get_regex_data('vid=(\w+)', url_content)
    print page_url, vid, title
  return

if __name__ == '__main__':
  main()

