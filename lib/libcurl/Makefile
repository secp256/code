# Makefile : Makefile for Crawler/prog
# CopyRight (c) 2008 Vobile, Inc.  All Rights Reserved.
# zhou_jiangang@voblie.cn
# 2008-05


TARGETS = ccurl_test

SRCS = $(foreach src, $(wildcard *.cpp), $(basename $(src)))
COMMOBJS=$(foreach src, $(filter-out $(TARGETS), $(SRCS)), $(src).o )

# test

CXX = g++

CFLAGS += -pipe -Wall -g

LDFLAGS += -lcurl

########################################
.PHONY : all clean

all : $(TARGETS)

$(TARGETS) : % : %.o $(COMMOBJS)
	$(CXX) $(CFLAGS) -o $@ $< $(COMMOBJS) $(LDFLAGS)

clean :
	rm -f $(TARGETS)
	rm -f $(COMMOBJS)
	rm -f $(foreach target, $(TARGETS), $(target).o)

##################################
.SUFFIXES : .cpp

.cpp.o :
	$(CXX) $(CFLAGS) -c -o $@ $<

