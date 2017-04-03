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

# General make(1) flags to build parts of tmail
MAKE_FLAGS=-s -C
# Main source directory
SRC_DIR=src/
# Directory with makefile auxiliary scripts
MK_DIR=mk/

# Include auxiliary build scripts
include $(MK_DIR)/Common.mk

# And include according build script
include $(MK_DIR)$(OS).mk

default: all

all:
	@$(MAKE) $(MAKE_FLAGS) $(SRC_DIR)

$(CLEAN_TARGET):
	@echo "./"
	$(QUIET_CLEAN_EXECS) $(RM) $(TMAIL_EXECUTABLE)
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@$(MAKE) $(MAKE_FLAGS) $(SRC_DIR) $(CLEAN_TARGET)

$(HELP_TARGET):
	@echo  'Common targets:'
	@echo  '  * all - Build everything.'
	@echo  'Cleaning targets:'
	@echo  '  * clean - Remove most generated files like object files, executables and etc.'
	@echo  'Miscellaneous targets'
	@echo  '  * help - print this output.'

.PHONY: $(CLEAN_TARGET) $(HELP_TARGET)
