#!/usr/bin/env python

from splinter import Browser
browser = Browser('chrome')

browser.visit('http://google.com')
browser.fill('q', 'splinter - python acceptance testing for web applications')

button = browser.find_by_name('btnG')
button.click()

browser.find_by_name('btnG').click()

if browser.is_text_present('splinter.readthedocs.org'):
  print "Yes, found it! :)"
else:
  print "No, didn't find it :("

browser.quit()
