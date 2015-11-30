#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ConfigParser

cf = ConfigParser.ConfigParser()
cf.read("conf.conf")

# list all sections
print cf.sections()

# list all items of section 'db'
print cf.items("db")

# get value of section 'db'
print cf.get("db", "db_host")
print cf.getint("db", "db_port")

# remove section
cf.remove_option("third", "pass")
cf.remove_section("third")

# add section
cf.add_section("third")
cf.set("third", "pass", "123")
cf.write(open("conf.conf", "w"))
