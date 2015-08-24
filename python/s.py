#!/usr/bin/python

import searchengine

e = searchengine.searcher('seindex.db')
#print e.get_all_words()
#print e.get_matchrows('python google')
e.query('python google')
