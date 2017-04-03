# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../mk/Common.mk
include ../mk/$(OS).mk

TMAIL_OBJS += tmail.o

default: all

all: $(TMAIL_OBJS)
	$(QUIET_CC) $(TMAIL_CC) -o ../$(TMAIL_EXECUTABLE) $(TMAIL_OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(CFLAGS) $@ $<

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)

.PHONY: $(CLEAN_TARGET)
