# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

.DEFAULT_GOAL: $(TMAIL_EXECUTABLE)

include ../mk/Common.mk
include ../mk/Build.mk

TMAIL_OBJS = tmail.o
TMAIL_OBJS += message.o
TMAIL_OBJS += send-email.o

# set of directories with C headers needed for tmail
INCLUDES+=-Ilibsys/
INCLUDES+=-Ilibutils/                    
INCLUDES+=-Ilibsmtp/
INCLUDES+=-I./

# libraries which are required by tmail
LIBS+=-ltmail-sys
LIBS+=-ltmail-utils
LIBS+=-ltmail-smtp

# paths to these libraries
# (we should add this at least to compile tmail
#  without installation of libraries to system)
LIBS_PATH+=-L$(shell pwd)/libsys
LIBS_PATH+=-L$(shell pwd)/libutils
LIBS_PATH+=-L$(shell pwd)/libsmtp

# Tell to linker where libraries are
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/libsys
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/libutils
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/libsmtp

COMPILER_FLAGS = $(LIBS_PATH) $(INCLUDES) $(LINKER_OPTS) $(CC_EXEC_FLAGS)

$(TMAIL_EXECUTABLE): $(TMAIL_OBJS)
	$(QUIET_CC) $(TMAIL_CC)	$(COMPILER_FLAGS) ../$@	$(TMAIL_OBJS) $(LIBS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@cd $(TMAIL_SYS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)
	@cd $(TMAIL_UTILS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)
	@cd $(TMAIL_SMTP_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)

.PHONY: $(CLEAN_TARGET)
