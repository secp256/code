#!/usr/bin/env python

from splinter.browser import Browser
import time
import  random
import sys

reload(sys)
sys.setdefaultencoding('utf-8')

def get_comment():
  fp = open('comment.txt', 'r')
  lines = fp.readlines()
  fp.close()
  return random.choice(lines)

def random_str(randomlength=8):
  str = ''
  chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789'
  length = len(chars) - 1
  for i in range(randomlength):
    str+=chars[random.randint(0, length)]
  return str

def random_sleep():
  interval = random.randint(0, 15)
  time.sleep(30 + interval)

url_template = "http://www.19lou.com/forum-9-%d.html"
for i in range(1, 40):
  url = url_template % i
  print url
  need_login = True
  with Browser() as browser:
    browser.visit(url)
    for i in browser.find_by_xpath('//div[@class="subject"]'):
      i.click()
      if len(browser.windows) < 2:
        continue
      time.sleep(5)
      browser.windows.current = browser.windows[1]
      if need_login:
        try:
          # login
          browser.fill('username', 'zhuliting')
          browser.fill('password', 'zhulitingok')
        except:
          pass
      else:
        need_login = False

      text = get_comment()
      browser.fill('content', text)
      browser.find_by_xpath('//a[@id="publishButton"]').first.click()
      random_sleep()
      browser.windows.current = browser.windows[0]
      if (len(browser.windows) > 1):
        browser.windows[1].close()
