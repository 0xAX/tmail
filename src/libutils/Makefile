# libutils Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../mk/Common.mk
include ../../mk/Build.mk

UTILS_OBJS += list.o
UTILS_OBJS += stack.o
UTILS_OBJS += hashmap.o
UTILS_OBJS += string_utils.o

TEST_EXECUTABLES += list_test
TEST_EXECUTABLES += stack_test
TEST_EXECUTABLES += hashmap_test

TEST_OBJS = list_test.o stack_test.o hashmap_test.o

all: $(UTILS_OBJS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(WARNINGS) $(OSX_FLAGS) $(CFLAGS_LIBS) $<

$(TEST_TARGET): $(TEST_EXECUTABLES)
	@exec ./run_tests.pl
	@rm -rf $(TEST_EXECUTABLES)

list_test:
	$(TMAIL_CC) -g $(OSX_FLAGS) $(DEBUG_FLAGS) -c list_test.c
	$(TMAIL_CC) -g $(OSX_FLAGS) $(DEBUG_FLAGS) list.o list_test.o -o list_test

stack_test:
	$(TMAIL_CC) -g $(OSX_FLAGS) $(DEBUG_FLAGS) -c stack_test.c
	$(TMAIL_CC) -g $(OSX_FLAGS) $(DEBUG_FLAGS) list.o stack.o stack_test.o -o stack_test

hashmap_test:
	$(TMAIL_CC) -g $(OSX_FLAGS) $(DEBUG_FLAGS) -c hashmap_test.c
	$(TMAIL_CC) -g $(OSX_FLAGS) $(DEBUG_FLAGS) hashmap.o hashmap_test.o -o hashmap_test

$(CLEAN_TARGET):
	@echo "./src/libutils"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(UTILS_OBJS)
	@rm -rf $(TEST_EXECUTABLES)
	@rm -rf $(TEST_OBJS)

.PHONY: $(CLEAN_TARGET)
