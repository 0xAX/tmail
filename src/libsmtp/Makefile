# libsmtp Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../mk/Common.mk
include ../../mk/Build.mk

INCLUDES += -I../../include
INCLUDES += $(OSX_FLAGS)

ifndef SSL_DISABLED
SMTP_OBJS += starttls.o
endif
SMTP_OBJS += attachment.o
SMTP_OBJS += ehlo.o
SMTP_OBJS += help.o
SMTP_OBJS += rcpt_to.o
SMTP_OBJS += mail_from.o
SMTP_OBJS += data_msg.o
SMTP_OBJS += body.o
SMTP_OBJS += quit.o
SMTP_OBJS += smtp.o
SMTP_OBJS += smtpauth.o

# some SMTP code uses libsys code
SYS_OBJS += ../libsys/gethostname.o

# some SMTP code uses libencoding code
ENCODING_OBJS += ../libencoding/base64.o

# Object files from tmail core
MIME_OBJECT = ../mime.o

# external libraries that are used by tmail
ifndef SSL_DISABLED
LIBS += $(OPENSSL)
endif

# Set of object files that we need to build ltmail-smtp.so
ALL_OBJS = $(SMTP_OBJS) $(SYS_OBJS) $(ENCODING_OBJS) $(MIME_OBJECT)

all: $(SMTP_OBJS) $(MIME_OBJECT)

$(MIME_OBJECT):
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(DEFS) $(WARNINGS) $(CFLAGS_LIBS) ../mime.c -o $(MIME_OBJECT)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(DEFS) $(INCLUDES) $(CFLAGS_LIBS) $<

$(CLEAN_TARGET):
	$(QUIET_CLEAN_OBJECTS) $(RM) $(SMTP_OBJS)
	$(QUIET_CLEAN_OBJECTS) $(RM) $(MIME_OBJECT)

.PHONY: $(CLEAN_TARGET)
