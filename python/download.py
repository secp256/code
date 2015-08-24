#!/usr/bin/env python

from splinter import Browser

class parser(object):
  def __init__(self):
    self.browser = Browser('phantomjs')

  def get_text_by_xpath(self, xpath):
    return self.browser.find_by_xpath(xpath).first.text

  def get_html_by_xpath(self, xpath):
    return self.browser.find_by_xpath(xpath).first.html

  def download_page(self, url):
    self.browser.visit(url)
    return self.browser.html

  def __del__(self):
    self.browser.quit


def main():
  p = parser()
  url = "http://v.youku.com/v_show/id_XNzU1MDcxMzI=.html"
  p.download_page(url)
  print p.browser.title
  # print p.browser.url
  print p.get_text_by_xpath("//div[@class='time']")
  print p.get_text_by_xpath("//*[@id='videoTotalPV']/em")

if __name__ == '__main__':
  main()
