# libencoding Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_ENCODING_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

LIB_ENCODING_OBJS += base64.o

TEST_EXECUTABLES += base64_test

TEST_OBJS += base64_test.o

$(TMAIL_ENCODING_LIB): $(LIB_ENCODING_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(CC_SHLIB_FLAGS) $@ $<

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(CFLAGS_LIBS) $<

$(TEST_TARGET): base64_test
	@exec ./run_tests.pl
	@rm -rf $(TEST_EXECUTABLES)

base64_test:
	$(TMAIL_CC) $(DEBUG_FLAGS) -c base64_test.c
	$(TMAIL_CC) $(DEBUG_FLAGS) base64.o base64_test.o -o base64_test

$(CLEAN_TARGET):
	@echo "./src/libencoding"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(LIB_ENCODING_OBJS)
	$(QUIET_CLEAN_SHARED_LIBS) $(RM) $(TMAIL_ENCODING_LIB)
	@rm -rf $(TEST_OBJS)
	@rm -rf $(TEST_EXECTUABLES)

.PHONY: $(CLEAN_TARGET)
