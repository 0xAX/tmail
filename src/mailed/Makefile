# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file
#
# For definitions, see root Makefile.

include ../../mk/Common.mk
include ../../mk/Build.mk

.DEFAULT_GOAL: $(MAIL_EDITOR_EXECUTABLE)

LIBS_PATH+=-L$(shell pwd)/../sys
LIBS_PATH+=-L$(shell pwd)/../libutils

LIBS+=-ltmail-sys
LIBS+=-ltmail-utils

LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/../sys/
LINKER_OPTS+=-Wl,-rpath,$(shell pwd)/../libutils

MAIL_EDITOR_OBJS = editor.o

INCLUDES=-I../sys/
INCLUDES+=-I../libutils/

COMPILER_FLAGS = $(LIBS_PATH) $(INCLUDES) $(LINKER_OPTS) $(CC_EXEC_FLAGS)

$(MAIL_EDITOR_EXECUTABLE): $(MAIL_EDITOR_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(COMPILER_FLAGS) ../../$@ $< $(LIBS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src/mailed"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)

.PHONY: $(CLEAN_TARGET)
