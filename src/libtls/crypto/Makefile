# libtls crypto Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../../mk/Common.mk
include ../../../mk/Build.mk

CRYPTO_OBJS += rsa.o

all: $(CRYPTO_OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(CFLAGS_LIBS) $<

$(CLEAN_TARGET):
	$(QUIET_CLEAN_OBJECTS) $(RM) $(CRYPTO_OBJS)

.PHONY: clean
