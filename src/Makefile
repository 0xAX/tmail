# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: tmail

include ../mk/Common.mk
include ../mk/Build.mk

TMAIL_OBJS = tmail.o

INCLUDE_DIRS = sys/

LIBS_PATH=-L$(shell pwd)/sys
LIBS=-ltmail-sys -lpthread
LINKER_OPTS=-Wl,-rpath,$(shell pwd)/sys
COMPILER_FLAGS= $(LINKER_OPTS) $(CC_EXEC_FLAGS)

tmail: $(TMAIL_SYS_LIB) $(TMAIL_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(LIBS_PATH) -I$(INCLUDE_DIRS) \
	$(COMPILER_FLAGS) ../$(TMAIL_EXECUTABLE) $(TMAIL_OBJS) $(LIBS)

$(TMAIL_SYS_LIB):
	@cd $(SYS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_SYS_LIB)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) -I$(INCLUDE_DIRS) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@cd $(SYS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)

.PHONY: $(CLEAN_TARGET)
