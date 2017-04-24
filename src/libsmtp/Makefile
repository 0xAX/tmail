# libsmtp Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_SMTP_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

OBJS = smtp.o

$(TMAIL_SMTP_LIB): $(OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(CC_SHLIB_FLAGS) $(TMAIL_SMTP_LIB) $(OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(CFLAGS_LIBS) $(BUILD_LIB_FLAGS) $<

$(CLEAN_TARGET):
	@echo "./src/libsmtp"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJS)
	$(QUIET_CLEAN_SHARED_LIBS) $(RM) $(TMAIL_SYS_LIB)

.PHONY: $(CLEAN_TARGET)