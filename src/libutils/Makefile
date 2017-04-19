# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_UTILS_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

OBJS = list.o

# test options
LINKER_TEST_FLAGS=-Wl,-rpath,$(shell pwd)
LIST_TEST_COMPILE=-g list_test.c -o list_test -ltmail-utils
TEST_EXECUTABLES=list_test

$(TMAIL_UTILS_LIB): $(OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(CC_SHLIB_FLAGS) $(TMAIL_UTILS_LIB) $(OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(CFLAGS_LIBS) $(BUILD_LIB_FLAGS) $<

$(TEST_TARGET):
	$(TMAIL_CC) $(LIST_TEST_COMPILE) $(LINKER_TEST_FLAGS)
	@exec ./run_tests.pl

$(CLEAN_TARGET):
	@echo "./src/libutils"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJS)
	$(QUIET_CLEAN_SHARED_LIBS) $(RM) $(TMAIL_SYS_LIB)
	@rm -rf $(TEST_EXECUTABLES)

.PHONY: $(CLEAN_TARGET)
