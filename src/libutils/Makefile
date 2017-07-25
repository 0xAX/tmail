# libutils Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: $(TMAIL_UTILS_LIB)

include ../../mk/Common.mk
include ../../mk/Build.mk

LIB_UTILS_OBJS += list.o
LIB_UTILS_OBJS += stack.o

TEST_EXECUTABLES += list_test
TEST_EXECUTABLES += stack_test

$(TMAIL_UTILS_LIB): $(LIB_UTILS_OBJS)
	$(QUIET_CC) $(TMAIL_CC) $(CC_SHLIB_FLAGS) $@ $<

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(CFLAGS_LIBS) $(BUILD_LIB_FLAGS) $<

$(TEST_TARGET):
	@rm -rf $(TEST_EXECUTABLES)
	$(TMAIL_CC) -ggdb -c list_test.c
	$(TMAIL_CC) -ggdb list.o list_test.o -o list_test
	$(TMAIL_CC) -ggdb -c stack_test.c
	$(TMAIL_CC) -ggdb list.o stack.o stack_test.o -o stack_test
	@exec ./run_tests.pl

$(CLEAN_TARGET):
	@echo "./src/libutils"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(LIB_UTILS_OBJS)
	$(QUIET_CLEAN_SHARED_LIBS) $(RM) $(TMAIL_UTILS_LIB)
	@rm -rf $(TEST_EXECUTABLES)

.PHONY: $(CLEAN_TARGET)
