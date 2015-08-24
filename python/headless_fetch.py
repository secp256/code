#!/usr/bin/env python

from splinter.browser import Browser
import time

url= "http://www.tvbts.com"
with Browser('phantomjs') as browser:
  browser.visit(url)
  # login
  browser.fill('username', 'zhuliting')
  browser.fill('password', '123456')
  browser.find_by_xpath('//button[@type="submit"]').first.click()

  #browser.click_link_by_href('/milist.html')
  browser.find_by_xpath('//*[@id="mn_N4233"]/a').first.click()
  print browser.url

  l = len(browser.find_by_xpath('//div[@class="pg"]/a'))
  for i in range(l):
    browser.find_by_xpath('//div[@class="pg"]/a')[i].click()
    print browser.url

    # save screenshot
    file_name = "/home/zhuliting/png/" + str(i) + ".png"
    browser.screenshot(file_name)

    contents = browser.find_by_xpath('//*[@class="s xst"]')
    for content in contents:
      print content.value
      # if (len(browser.windows) > 1):
      #   browser.windows[1].close()
