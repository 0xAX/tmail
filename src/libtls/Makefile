# libtls Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_TLS_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

INCLUDES += -I../
INCLUDES += -I../libutils/
INCLUDES += -I../libsys/
INCLUDES += -I../libencoding/

LIB_TLS_OBJS += handshake.o
LIB_TLS_OBJS += tls_utils.o
LIB_TLS_OBJS += tls.o

$(TMAIL_TLS_LIB): $(LIB_TLS_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CC_SHLIB_FLAGS) $@ $<

$(MIME_OBJECT):
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_LIBS) ../mime.c -o $(MIME_OBJECT)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_LIBS) $<

$(CLEAN_TARGET):
	$(QUIET_CLEAN_OBJECTS) $(RM) $(LIB_TLS_OBJS)

.PHONY: $(CLEAN_TARGET)
