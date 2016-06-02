#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import json
import urllib2
import re
import random
import time

reload(sys)
sys.setdefaultencoding('utf-8') 

PRODUCE_URL_FILE = "product_url.txt" # product url file
# DETAIL_INFO_FILE = "detail_info.txt" # product detail info file

def random_sleep(interval = 3):
  time.sleep(random.randint(0,interval))
  return

def get_regex_data(regex, buf):
  group = re.search(regex, buf)
  if group:
    return group.groups()[0]
  else:
    return ''

def download_page(url, data = None, headers = {}):
  request = urllib2.Request(url, data)

  # set headers
  for key, value in headers.items():
    request.add_header(key, value)

  opener = urllib2.build_opener()
  response = opener.open(request, timeout=100)
  content = response.read()
  return content

def crawler():
  '''
  get all product url from meifujia app server
  '''
  headers = {
      'User-Agent': 'Android4.2.2',
      'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
      }

  url = 'http://www.caimiapp.com/api_240/product/getProductLibraryList'

  id_set = set([])
  num = 0
  while True:
    post_data = 'fid=0&bid=0&cid=0&price_id=0&eid=0&selectType=0&total=%d&skinCode=----&token=' % num
    print post_data
    page_content = download_page(url, post_data, headers)
    js = json.loads(page_content)
    id_list = js['data']['list']

    # get product id
    dup_count = 0
    for item in  id_list:
      item_id = item['id']
      if item_id in id_set:
        dup_count += 1
      else:
        # print item_id
        id_set.add(item_id)

    print 'current [%d] products' % len(id_set)
    # no new id
    if dup_count == len(id_list):
      print "search end"
      break

    # go to next page
    num += 20
    random_sleep(3)

  print 'total [%d] products' % len(id_set)

  # write file
  fp = open(PRODUCE_URL_FILE, 'w')
  for i in id_set:
    product_url =  'http://www.caimiapp.com/cpxqq/?id=%d&skin=1200&source=share' % i
    # print product_url
    fp.write(product_url + '\n')
  fp.close()
  return

def process():
  '''
  get all product detail info
  '''
  # http://www.caimiapp.com/api_240/product/getProductElement?id=182603&token=&skinCode=1200
  # http://www.caimiapp.com/api_240/product/getProductUserReviewList?pid=182603&token=&type=top10
  fp = open(PRODUCE_URL_FILE, 'r')
  lines = fp.readlines()
  
  for line in lines:
    line = line.strip()
    print 'product url: %s' % line
    product_id = get_regex_data('id=(\d+)', line)
    # print product_id

    # get elements
    detail_url = 'http://www.caimiapp.com/api_240/product/getProductElement?id=%d&token=&skinCode=1200' % int(product_id)
    detail_buf = download_page(detail_url)
    js = json.loads(detail_buf)
    data_list = js['data']['list'][0]['elementList']
    print 'elements: ',
    for i in data_list:
      print i['elementChinaName'].strip(),
      print '|',
    print

    # get comment
    comment_url = 'http://www.caimiapp.com/api_240/product/getProductUserReviewList?pid=%d&token=&type=top10' % int(product_id)
    # print comment_url
    comment_buf = download_page(comment_url)
    js = json.loads(comment_buf)
    tmp_list = js['data']['list']
    if len(tmp_list) > 0:
      data_list = tmp_list
    else:
      continue

    i_count = 1
    for i in data_list:
      print 'comment %d: %s' % (i_count, i['commentContent'])
      print 'skin %d: %s' % (i_count, i['user']['skin'])
      i_count += 1
    
    random_sleep(3)
    print '----------------------------------------------'
    print

  fp.close()
  return

def print_help():
  print 'usage:'
  print 'pyhthon %s crawler  # crawler url from app' % sys.argv[0]
  print 'pyhthon %s detail   # get detail info from crawler result' % sys.argv[0]
  return

def main():
  if len(sys.argv) < 2:
    print_help()
    sys.exit(1)

  if "crawler" == sys.argv[1]:
    crawler()
  else:
    process()

if __name__ == "__main__":
  main()
