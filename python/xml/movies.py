#!/usr/bin/env python
# -*- coding: utf-8 -*-

from xml.dom.minidom import parse
import xml.dom.minidom

DOMTree = xml.dom.minidom.parse('./data/movies.xml')
collection = DOMTree.documentElement
if collection.hasAttribute('shelf'):
  print 'Root element: %s' % collection.getAttribute('shelf')

# get all movie from collection
movies = collection.getElementsByTagName('movie')

# print each movie info
for movie in movies:
  print '*** movie ***'
  if movie.hasAttribute('title'):
    print 'title: %s' % movie.getAttribute('title')

  type = movie.getElementsByTagName('type')[0]
  print 'type: %s' % type.childNodes[0].data

