#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
import time
import datetime
import codecs
import multiprocessing as mp
from os import makedirs
from os.path import exists
from selenium import webdriver
from selenium.webdriver.common.proxy import *

url = 'http://www.newzhong.com/hjys/316.html'

proxy='192.168.200.252:3127'
proxy = Proxy({
    'proxyType': ProxyType.MANUAL,
    'httpProxy': proxy,
    'ftpProxy': proxy,
    'sslProxy': proxy,
    'noProxy': '' # 过滤不需要代理的地址
})
# use proxy
# driver = webdriver.Firefox(proxy=proxy)
driver = webdriver.Firefox()
driver.get(url)
#driver.maximize_window() # 将浏览器最大化显示
#driver.find_element_by_xpath('//button[@type="submit"]').click()
#driver.find_element_by_id('nextI3')
list_xpath = '//ul[@class="c1 ico2"]/li/a'
list_ad = driver.find_elements_by_xpath('.//*[@id="container"]//div[@class="title"]')
print len(list_ad)
items = driver.find_elements_by_xpath(list_xpath)
l = len(items)
for i in range(l):
  items = driver.find_elements_by_xpath(list_xpath)
  item = items[i]
  url = item.get_attribute('href').encode('utf8')
  text = item.text
  print url, text
  item.click();
  time.sleep(1)
  driver.back()

time.sleep(2)
driver.close()
