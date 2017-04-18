# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file
#
# Define TMAIL_CC to override default compiler (gcc)
#
# Define V=1 for a more verbose build
#
# Define DEBUG=1 to build tmail with debug info.

default: all

# Include auxiliary build scripts
include ./mk/Common.mk
# And include according build script
include ./mk/Build.mk

all:
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) tmail

$(CLEAN_TARGET):
	@echo "./"
	$(QUIET_CLEAN_EXECS) $(RM) $(TMAIL_EXECUTABLE)
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)

$(TEST_TARGET):
	@cd $(SRC_DIR)/$(UTILS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)

$(HELP_TARGET):
	@echo  'Common targets:'
	@echo  '  * all - Build everything.'
	@echo  '  * test - Run all tests.'
	@echo  'Cleaning targets:'
	@echo  '  * clean - Remove most generated files like object files, executables and etc.'
	@echo  'Miscellaneous targets'
	@echo  '  * help - print this output.'

.PHONY: $(CLEAN_TARGET) $(HELP_TARGET)
