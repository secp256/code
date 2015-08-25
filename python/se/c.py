#!/usr/bin/python

import searchengine

pages = ['https://www.python.org/']
crawler = searchengine.crawler('index.db')
crawler.createindextables()
crawler.crawl(pages)
crawler.calculate_page_rank()
#cur=crawler.con.execute('select * from pagerank order by score desc')
#for i in range(3): print cur.next( )
