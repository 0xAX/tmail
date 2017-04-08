# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: tmail

include ../mk/Common.mk
include ../mk/Build.mk

TMAIL_OBJS = tmail.o

INCLUDE_DIRS = sys/

LIBS=-Lsys/ -ltmail-sys
LINKER_OPTS=-Wl,-rpath,$(shell pwd)/sys
COMPILER_FLAGS= $(LINKER_OPTS) $(LIBS) $(CC_EXEC_FLAGS)

tmail: $(TMAIL_SYS_LIB) $(TMAIL_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(COMPILER_FLAGS) ../$(TMAIL_EXECUTABLE) $(TMAIL_OBJS)

$(TMAIL_SYS_LIB):
	@$(MAKE) $(MAKE_FLAGS) $(SYS_DIR_NAME) $(TMAIL_SYS_LIB)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) -I$(INCLUDE_DIRS) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@$(MAKE) $(MAKE_FLAGS) $(SYS_DIR_NAME) $(CLEAN_TARGET)

.PHONY: $(CLEAN_TARGET)
