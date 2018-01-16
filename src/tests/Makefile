# tmail core tests
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../mk/Common.mk
include ../../mk/Build.mk

TEST_OBJS += mime_test.o
TEST_OBJS += config_test.o

TEST_EXECUTABLES += mime_test
TEST_EXECUTABLES += config_test

INCLUDE_DIR += -I../
INCLUDE_DIR += -I../libutils
INCLUDE_DIR += -I../libsmtp
INCLUDE_DIR += -I../libencoding
INCLUDE_DIR += -I../libsys

LIB_DIR = -L../libsmtp/

$(TEST_TARGET): $(TEST_EXECUTABLES)
	@exec ./run_tests.pl
	@rm -rf $(TEST_EXECUTABLES)

mime_test:
	$(TMAIL_CC) $(OSX_FLAGS) $(INCLUDE_DIR) $(DEBUG_FLAGS) -c mime_test.c
	$(TMAIL_CC) $(OSX_FLAGS) $(INCLUDE_DIR) $(DEBUG_FLAGS) ../mime.o mime_test.o -o mime_test

config_test:
	@echo $(shell pwd)
	$(TMAIL_CC) $(LIB_DIR) $(OSX_FLAGS) $(INCLUDE_DIR) $(DEBUG_FLAGS) -c config_test.c
	$(TMAIL_CC) $(LIB_DIR) $(OSX_FLAGS) $(INCLUDE_DIR) $(DEBUG_FLAGS) config_test.o -o config_test

$(CLEAN_TARGET):
	@echo "./src/tests"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(TEST_OBJS)
	@rm -rf $(TEST_EXECUTABLES)

.PHONY: $(CLEAN_TARGET)
