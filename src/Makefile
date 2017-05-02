# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

.DEFAULT_GOAL: $(TMAIL_EXECUTABLE)

include ../mk/Common.mk
include ../mk/Build.mk

TMAIL_OBJS = tmail.o

# set of directories with C headers needed for tmail
INCLUDES+=-Isys/
INCLUDES+=-Ilibutils/                    
INCLUDES+=-Ilibsmtp/

# libraries which are required by tmail
LIBS+=-ltmail-sys
LIBS+=-ltmail-utils
LIBS+=-ltmail-smtp

# paths to these libraries
# (we should add this at least to compile tmail
#  without installation of libraries to system)
LIBS_PATH+=-L$(shell pwd)/sys
LIBS_PATH+=-L$(shell pwd)/libutils
LIBS_PATH+=-L$(shell pwd)/libsmtp

# Tell to linker where libraries are
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/sys
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/libutils
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/libsmtp

COMPILER_FLAGS = $(LIBS_PATH) $(INCLUDES) $(LINKER_OPTS) $(CC_EXEC_FLAGS)

TMAIL_EXECUTABLE_DEPS+=$(TMAIL_SYS_LIB)
TMAIL_EXECUTABLE_DEPS+=$(TMAIL_UTILS_LIB)
TMAIL_EXECUTABLE_DEPS+=$(TMAIL_SMTP_LIB)
TMAIL_EXECUTABLE_DEPS+=$(TMAIL_OBJS)

$(TMAIL_EXECUTABLE): $(TMAIL_EXECUTABLE_DEPS)
	$(QUIET_CC) $(TMAIL_CC)	$(COMPILER_FLAGS) ../$(TMAIL_EXECUTABLE) \
	$(TMAIL_OBJS) $(LIBS)

$(TMAIL_SMTP_LIB):
	@cd $(TMAIL_SMTP_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_SMTP_LIB)

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
