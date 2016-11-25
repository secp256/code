#!/usr/bin/python
#-*- coding: utf-8 -*-
#encoding=utf-8

import urllib2
import urllib
import os
from BeautifulSoup import BeautifulSoup
import time
import sys

def cbk(a, b, c): 
  '''回调函数
  @a: 已经下载的数据块
  @b: 数据块的大小
  @c: 远程文件的大小
  ''' 
  per = 100.0 * a * b / c 
  if per > 100: 
      per = 100 
  print '%.2f%%' % per

def getAllImageLink():
  html = urllib2.urlopen('http://www.dbmeizi.com').read()
  soup = BeautifulSoup(html)

  liResult = soup.findAll('li',attrs={"class":"span3"})

  for li in liResult:
    imageEntityArray = li.findAll('img')
    for image in imageEntityArray:
        link = image.get('data-src')
        imageName = image.get('data-id')
        filesavepath = '/home/zhuliting/meizipicture/%s.jpg' % imageName
        urllib.urlretrieve(link, filesavepath, cbk)
        print filesavepath

if __name__ == '__main__':
  #getAllImageLink()
  for i in range(1,100):
    time.sleep(1)
    print '%.2d' % i,
    sys.stdout.flush()
