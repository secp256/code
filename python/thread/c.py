#!/usr/bin/env python
# -*- coding: utf-8 -*-

def funA(a):
    print 'funA'

def funB(b):
    print 'funB'

@funA
@funB
def funC():
    print 'funC'
