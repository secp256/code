#!/usr/bin/env python
# -*- coding: utf-8 -*-

import xml.etree.ElementTree as ET
tree = ET.parse("./data/xml_etree_data.xml")
root = tree.getroot()
# find
for country in root.findall("country"):
  rank = country.find('rank')
  # rank = country.find('rank').text
  print rank
