#!/usr/bin/env python
# -*- coding: utf-8 -*-

import xml.etree.ElementTree as ET
tree = ET.parse("xml_etree_data.xml")
root = tree.getroot()
# root = ET.fromstring(country_data_as_string)
print "root tag: ", root.tag
for child in root:
  print child.tag, child.attrib

# access specific child nodes by index
print root[0][1].text

# iterate recursively over the sub-tree
for neighbor in root.iter("neighbor"):
  print neighbor.attrib

# find
for country in root.findall("country"):
  rank = country.find('rank').text
  name = country.get('name')
  print rank, name

# xpath
for i in root.findall(".//*[@name='Singapore']/year"):
  print "find by xpath: ", i.text
