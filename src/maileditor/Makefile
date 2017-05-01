# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file
#
# For definitions, see root Makefile.

include ../../mk/Common.mk
include ../../mk/Build.mk

.DEFAULT_GOAL: $(MAIL_EDITOR_EXECUTABLE)

LIBS_PATH=-L$(shell pwd)/../sys		\
	  -L$(shell pwd)/../libutils

LIBS=-ltmail-sys	\
     -ltmail-utils

LINKER_OPTS=-Wl,-rpath,$(shell pwd)/../sys/	\
	    -Wl,-rpath,$(shell pwd)/../libutils

MAIL_EDITOR_OBJS = editor.o

INCLUDES=-I../sys/	\
	 -I../libutils/

COMPILER_FLAGS= $(LINKER_OPTS) $(CC_EXEC_FLAGS)

$(MAIL_EDITOR_EXECUTABLE): $(MAIL_EDITOR_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(LIBS_PATH) $(INCLUDES) $(COMPILER_FLAGS) ../../$(MAIL_EDITOR_EXECUTABLE) $(MAIL_EDITOR_OBJS) $(LIBS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src/maileditor"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)

.PHONY: $(CLEAN_TARGET)
