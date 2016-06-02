#!/usr/bin/env python
# -*- coding: utf-8 -*-

import optparse

def main():
  usage = "usage"
  parser = optparse.OptionParser(usage=usage)
  parser.add_option("-u", "--url",
      dest = "url",
      default = "http://www.baidu.com",
      help="start the domain name"
      )
  (options, args) = parser.parse_args()
  url = options.url

  return

if __name__ == "__main__":
  main()
