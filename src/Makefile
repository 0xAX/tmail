# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: tmail

include ../mk/Common.mk
include ../mk/Build.mk

TMAIL_OBJS = tmail.o

INCLUDES=-Isys/ -Ilibutils/
LIBS_PATH=-L$(shell pwd)/sys -L$(shell pwd)/libutils
LIBS=-ltmail-sys -lpthread -ltmail-utils
LINKER_OPTS=-Wl,-rpath,$(shell pwd)/sys \
            -Wl,-rpath,$(shell pwd)/libutils
COMPILER_FLAGS= $(LINKER_OPTS) $(CC_EXEC_FLAGS)

tmail: $(TMAIL_SYS_LIB) $(TMAIL_UTILS_LIB) $(TMAIL_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(LIBS_PATH) $(INCLUDES) \
	$(COMPILER_FLAGS) ../$(TMAIL_EXECUTABLE) $(TMAIL_OBJS) $(LIBS)

$(TMAIL_UTILS_LIB):
	@cd $(UTILS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_UTILS_LIB)

$(TMAIL_SYS_LIB):
	@cd $(SYS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_SYS_LIB)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@cd $(SYS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)
	@cd $(UTILS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)

.PHONY: $(CLEAN_TARGET)
