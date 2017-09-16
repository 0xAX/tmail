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
# Define LIBDIR to specify where to install tmail shared libraries
#
# Define BINDIR to specify where to install tmail executables
#
# Define MANDIR to specify where to install tmail man pages
.DEFAULT_GOAL: $(DEFAULT_TARGET)

include ./mk/Common.mk
include ./mk/Build.mk

# root directory
TOPDIR=$(shell pwd)

$(DEFAULT_TARGET): $(BUILD_LIBS_TARGET)
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_EXECUTABLE) TOPDIR=$(TOPDIR)

$(BUILD_LIBS_TARGET): $(BUILD_TLS_LIB_TARGET) \
		      $(BUILD_SYS_LIB_TARGET) \
		      $(BUILD_UTILS_LIB_TARGET) \
                      $(BUILD_ENCODING_LIB_TARGET) \
                      $(BUILD_SMTP_LIB_TARGET)

$(BUILD_TLS_LIB_TARGET):
	@cd $(SRC_DIR)$(TMAIL_TLS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_TLS_LIB) TOPDIR=$(TOPDIR)

$(BUILD_SYS_LIB_TARGET):
	@cd $(SRC_DIR)$(TMAIL_SYS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_SYS_LIB) TOPDIR=$(TOPDIR)

$(BUILD_ENCODING_LIB_TARGET):
	@cd $(SRC_DIR)$(TMAIL_ENCODING_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_ENCODING_LIB) TOPDIR=$(TOPDIR)

$(BUILD_SMTP_LIB_TARGET):
	@cd $(SRC_DIR)$(TMAIL_SMTP_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_SMTP_LIB) TOPDIR=$(TOPDIR)

$(BUILD_UTILS_LIB_TARGET):
	@cd $(SRC_DIR)$(TMAIL_UTILS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_UTILS_LIB) TOPDIR=$(TOPDIR)

$(TEST_TARGET): $(BUILD_LIBS_TARGET)
	@cd $(SRC_DIR)$(TMAIL_UTILS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)
	@cd $(SRC_DIR)$(TMAIL_ENCODING_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)
	@cd $(SRC_DIR)tests && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)

$(INSTALL_TARGET): $(BUILD_LIBS_TARGET)
	@echo "Installing tmail..."
	@$(INSTALL) $(TMAIL_EXECUTABLE) $(BIN_DIR)
	@echo "Installing libraries..."
	@$(INSTALL) $(SRC_DIR)$(TMAIL_UTILS_LIB_DIR)/$(TMAIL_UTILS_LIB) $(LIB_DIR)
	@$(INSTALL) $(SRC_DIR)$(TMAIL_SYS_LIB_DIR)/$(TMAIL_SYS_LIB) $(LIB_DIR)
	@$(INSTALL) $(SRC_DIR)$(TMAIL_SMTP_LIB_DIR)/$(TMAIL_SMTP_LIB) $(LIB_DIR)
	@$(INSTALL) $(SRC_DIR)$(TMAIL_ENCODING_LIB_DIR)/$(TMAIL_ENCODING_LIB) $(LIB_DIR)
	@$(INSTALL) $(SRC_DIR)$(TMAIL_TLS_LIB_DIR)/$(TMAIL_TLS_LIB) $(LIB_DIR)
	@$(INSTALL) $(MAN_PAGES_1) $(MAN_DIR_1)
	@$(INSTALL) $(MAN_PAGES_5) $(MAN_DIR_5)
	@echo "Done."

$(UNINSTALL_TARGET):
	@rm -rf $(BIN_DIR)/$(TMAIL_EXECUTABLE)
	@rm -rf $(LIB_DIR)/$(TMAIL_SMTP_LIB)
	@rm -rf $(LIB_DIR)/$(TMAIL_SYS_LIB)
	@rm -rf $(LIB_DIR)/$(TMAIL_UTILS_LIB)
	@rm -rf $(LIB_DIR)/$(TMAIL_ENCODING_LIB)
	@rm -rf $(LIB_DIR)/$(TMAIL_TLS_LIB)
	@rm -rf $(MAN_DIR_1)/tmail*
	@rm -rf $(MAN_DIR_5)/tmail*
	@echo "Done."

$(CLEAN_TARGET):
	@echo "./"
	$(QUIET_CLEAN_TMAIL_EXEC) $(RM) $(TMAIL_EXECUTABLE)
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET) TOPDIR=$(TOPDIR)

$(HELP_TARGET):
	@echo  'Common targets:'
	@echo  '  * all       - Build everything.'
	@echo  '  * test      - Run all tests.'
	@echo  '  * install   - Install tmail with tools and libraries.'
	@echo  '  * uninstall - Uninstall tmail with tools and libraries.'
	@echo  ''
	@echo  'Libraries targets:'
	@echo  '  * libs - build all shared libraries.'
	@echo  ''
	@echo  'Cleaning targets:'
	@echo  '  * clean     - Remove most generated files like object files, executables and etc.'
	@echo  ''
	@echo  'Miscellaneous targets'
	@echo  '  * help      - print this output.'

.PHONY: $(CLEAN_TARGET) $(HELP_TARGET) $(INSTALL_TARGET) $(UNINSTALL_TARGET)
