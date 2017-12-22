# libsys Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_SYS_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

INCLUDES += -I../libutils/ $(OSX_FLAGS)

LIB_SYS_OBJS += at_exit.o
LIB_SYS_OBJS += connect.o
LIB_SYS_OBJS += fcntl_utils.o
LIB_SYS_OBJS += gethostname.o
LIB_SYS_OBJS += inet.o

$(TMAIL_SYS_LIB): $(LIB_SYS_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(WARNINGS) $(CC_SHLIB_FLAGS) $@ $<

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(WARNINGS) $(CFLAGS_LIBS) $<

$(CLEAN_TARGET):
	@echo "./src/libsys"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(LIB_SYS_OBJS)
	$(QUIET_CLEAN_SHARED_LIBS) $(RM) $(TMAIL_SYS_LIB)

.PHONY: $(CLEAN_TARGET)
