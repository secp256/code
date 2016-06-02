#!/usr/bin/env python

from selenium import webdriver

browser = webdriver.Firefox()
url = "http://codingpy.com"
browser.set_window_size(1200, 900)
browser.get(url)

browser.save_screenshot("codingpy.png")
browser.close()
