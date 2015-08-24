#!/usr/bin/python

import searchengine

pages = ['https://www.python.org/']
crawler = searchengine.crawler('seindex.db')
#crawler.createindextables()
crawler.crawl(pages)
