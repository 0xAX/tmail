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
#
# Define NON_NATIVE_BINARY to disable -march=native
#

.DEFAULT_GOAL: $(DEFAULT_TARGET)

# Include auxiliary build scripts
include ./mk/Common.mk
# And include according build script
include ./mk/Build.mk

$(DEFAULT_TARGET):
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_EXECUTABLE)
	@cd $(SRC_DIR)/$(MAIL_EDITOR_DIR) && $(MAKE) $(MAKE_FLAGS)

$(TMAIL_EXECUTABLE):
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_EXECUTABLE)

$(MAIL_EDITOR_EXECUTABLE):
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_UTILS_LIB)
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_SYS_LIB)
	@cd $(SRC_DIR)/$(MAIL_EDITOR_DIR) && $(MAKE) $(MAKE_FLAGS)

$(CLEAN_TARGET):
	@echo "./"
	$(QUIET_CLEAN_TMAIL_EXEC) $(RM) $(TMAIL_EXECUTABLE)
	$(QUIET_CLEAN_MAILED_EXEC) $(RM) $(MAIL_EDITOR_EXECUTABLE)
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)
	@cd $(SRC_DIR)/$(MAIL_EDITOR_DIR) && $(MAKE) $(MAKE_FLAGS) \
	$(CLEAN_TARGET)

$(TEST_TARGET):
	@cd $(SRC_DIR)/$(UTILS_DIR_NAME) && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)

$(INSTALL_TARGET): all
	@$(INSTALL) $(TMAIL_EXECUTABLE) $(BIN_DIR)
	@$(INSTALL) $(SRC_DIR)$(UTILS_DIR_NAME)/$(TMAIL_UTILS_LIB) $(LIB_DIR)
	@$(INSTALL) $(SRC_DIR)$(SYS_DIR_NAME)/$(TMAIL_SYS_LIB) $(LIB_DIR)

$(UNINSTALL_TARGET):
	rm -rf $(BIN_DIR)/$(TMAIL_EXECUTABLE)
ifdef TMAIL_UTILS_LIB
	rm -rf $(LIB_DIR)/$(TMAIL_UTILS_LIB)
else
	$(warning "WARN: libtmail-utils.so is not installed")
endif
ifdef TMAIL_SYS_LIB
	rm -rf $(LIB_DIR)/$(TMAIL_SYS_LIB)
else
	$(warning "WARN: libtmail-sys.so is not installed")
endif

$(HELP_TARGET):
	@echo  'Common targets:'
	@echo  '  * all       - Build everything.'
	@echo  '  * test      - Run all tests.'
	@echo  '  * install   - Install tmail with tools and libraries.'
	@echo  '  * uninstall - Uninstall tmail with tools and libraries.'
	@echo  ''
	@echo  'Build and install mail tools:'
	@echo  '  * mailed - build mailed email editor executable.'
	@echo  ''
	@echo  'Cleaning targets:'
	@echo  '  * clean     - Remove most generated files like object files, executables and etc.'
	@echo  ''
	@echo  'Miscellaneous targets'
	@echo  '  * help      - print this output.'

.PHONY: $(CLEAN_TARGET) $(HELP_TARGET)
