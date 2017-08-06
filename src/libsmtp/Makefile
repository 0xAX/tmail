# libsmtp Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_SMTP_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

INCLUDES += -I../
INCLUDES += -I../libutils/
INCLUDES += -I../libsys/
INCLUDES += -I../libencoding/

LIB_SMTP_OBJS += ehlo.o
LIB_SMTP_OBJS += rcpt_to.o
LIB_SMTP_OBJS += mail_from.o
LIB_SMTP_OBJS += data_msg.o
LIB_SMTP_OBJS += body.o
LIB_SMTP_OBJS += quit.o
LIB_SMTP_OBJS += smtp.o

# some SMTP code uses libsys code
LIB_SYS_OBJS += ../libsys/gethostname.o

# some SMTP code uses libencoding code
LIB_ENCODING_OBJS += ../libencoding/base64.o

$(TMAIL_SMTP_LIB): $(LIB_SMTP_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CC_SHLIB_FLAGS) $@ $< $(LIB_SYS_OBJS) $(LIB_ENCODING_OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_LIBS) $(BUILD_LIB_FLAGS) $<

$(CLEAN_TARGET):
	$(QUIET_CLEAN_OBJECTS) $(RM) $(LIB_SMTP_OBJS)
	$(QUIET_CLEAN_SHARED_LIBS) $(RM) $(TMAIL_SMTP_LIB)

.PHONY: $(CLEAN_TARGET)
