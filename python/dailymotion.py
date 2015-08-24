#!/usr/bin/python

import urllib2

def download_page(url, proxy = None, referer = None):
  page_buf = ''
  try:
    # set http proxy
    if proxy:
      handlers = [urllib2.ProxyHandler({'http': 'http://%s/' % proxy})]
      opener =  urllib2.build_opener(*handlers)
    else:
      opener   =  urllib2.build_opener()

    method   =  urllib2.Request(url)
    # set HTTP Referer
    if referer:
      method.add_header('Referer', referer)

    # add user agent
    method.add_header('User-Agent', 'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36')
    method.add_header('Accept-Language', 'en-US,en;q=0.5')

    result   =  opener.open(method, timeout=100)
    page_buf =  result.read()
  except Exception, reason:
    print 'download failed: ', reason

  return page_buf

def main():
  url = "http://www.dailymotion.com/video/x1o34aw"
  page_buf = download_page(url, '192.168.200.253:3128')
  if "Set Age Gate OFF" in page_buf:
    print 'Set Age Gate OFF'
    gate_url = "http://family-filter.dailymotion.com/disable?urlback=/video/" + "x1o34aw"
    print gate_url
    page_buf = download_page(gate_url, '192.168.200.253:3128')
  print page_buf

if __name__ == '__main__':
  main()
