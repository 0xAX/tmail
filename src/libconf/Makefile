# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

default: test

include ../../mk/Common.mk
include ../../mk/Build.mk

INCLUDE=-I.
TEST=config_test.c

test:
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDE) $(TEST) $(CC_EXEC_FLAGS) \
	$(LIBCONF_TEST_EXEC)

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	$(QUIET_CLEAN_EXECS) $(RM) $(LIBCONF_TEST_EXEC)

.PHONY: $(CLEAN_TARGET)
