# libtls Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_TLS_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

CRYPTO_MODULES = crypto/

INCLUDES += -I../
INCLUDES += -I./crypto/
INCLUDES += -I../libutils/
INCLUDES += -I../libsys/
INCLUDES += -I../libencoding/

CRYPTO_MODULES_OBJS += ./crypto/rsa.o

LIB_TLS_OBJS += tls_utils.o
LIB_TLS_OBJS += tlskey.o
LIB_TLS_OBJS += certificate.o
LIB_TLS_OBJS += handshake.o
LIB_TLS_OBJS += tls.o

$(TMAIL_TLS_LIB): $(CRYPTO_MODULES_OBJS) $(LIB_TLS_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CC_SHLIB_FLAGS) $@ $(CRYPTO_MODULES_OBJS) $(LIB_TLS_OBJS)

$(MIME_OBJECT):
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_LIBS) ../mime.c -o $(MIME_OBJECT)

$(CRYPTO_MODULES_OBJS):
	@cd crypto && $(MAKE) $(MAKE_FLAGS) all

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_LIBS) $<

$(CLEAN_TARGET):
	$(QUIET_CLEAN_OBJECTS) $(RM) $(LIB_TLS_OBJS)
	$(QUIET_CLEAN_OBJECTS) $(RM) $(CRYPTO_MODULES_OBJS)

.PHONY: $(CLEAN_TARGET)
