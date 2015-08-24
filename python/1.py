#!/usr/bin/env python

from splinter.browser import Browser

url= "http://www.tvbts.com"
with Browser('phantomjs') as browser:
  browser.visit(url)
  print browser.url

  href = "http://www.tvbts.com/forum-50-1.html"
  # browser.click_link_by_href(href)
  print browser.url

