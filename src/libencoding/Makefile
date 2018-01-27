# libencoding Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../mk/Common.mk
include ../../mk/Build.mk

ENCODING_OBJS += base64.o

TEST_EXECUTABLES += base64_test

TEST_OBJS += base64_test.o

INCLUDES = -I../../include -I../libutils $(OSX_FLAGS)

all: $(ENCODING_OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(WARNINGS) $(CFLAGS_LIBS) $<

$(TEST_TARGET): base64_test
	@exec ./run_tests.pl
	@rm -rf $(TEST_EXECUTABLES)

base64_test:
	$(TMAIL_CC) -g $(INCLUDES) $(DEBUG_FLAGS) -c base64_test.c
	$(TMAIL_CC) -g $(INCLUDES) $(DEBUG_FLAGS) base64.o base64_test.o -o base64_test

$(CLEAN_TARGET):
	@echo "./src/libencoding"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(ENCODING_OBJS)
	@rm -rf $(TEST_OBJS)
	@rm -rf $(TEST_EXECUTABLES)

.PHONY: $(CLEAN_TARGET)
