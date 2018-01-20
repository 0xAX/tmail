# libsys Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../mk/Common.mk
include ../../mk/Build.mk

INCLUDES += -I../libutils/ $(OSX_FLAGS)

SYS_OBJS += at_exit.o
SYS_OBJS += connect.o
SYS_OBJS += fcntl_utils.o
SYS_OBJS += gethostname.o
SYS_OBJS += inet.o

all: $(SYS_OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(WARNINGS) $(CFLAGS_LIBS) $<

$(CLEAN_TARGET):
	@echo "./src/libsys"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(SYS_OBJS)

.PHONY: $(CLEAN_TARGET) $(TMAIL_SYS)
