# tmail Makefile
#
# Copyright (C) 2017-2019 Alexander Kuleshov <kuleshovmail at gmail dot com>
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
# Define BINDIR to specify where to install tmail executables
#
# Define MANDIR to specify where to install tmail man pages
#
# Define SSL_DISABLED=1 to build tmail without openssl.
#
# Following flags can be passed too, but they are mostly CI,
# containerization and testing stuff
.DEFAULT_GOAL: $(DEFAULT_TARGET)

include ./mk/Common.mk
include ./mk/Build.mk

# root directory
TOPDIR=$(shell pwd)

$(DEFAULT_TARGET): $(BUILD_SUB_TARGET)
	@cd $(SRC_DIR) && $(MAKE) $(MAKE_FLAGS) $(TMAIL_EXECUTABLE) TOPDIR=$(TOPDIR)

$(BUILD_SUB_TARGET): $(BUILD_SYS_TARGET) $(BUILD_UTILS_TARGET) \
		     $(BUILD_ENCODING_TARGET) $(BUILD_SMTP_TARGET)

$(BUILD_SYS_TARGET):
	@cd $(SRC_DIR)$(TMAIL_SYS_DIR) && $(MAKE) $(MAKE_FLAGS) TOPDIR=$(TOPDIR)

$(BUILD_ENCODING_TARGET):
	@cd $(SRC_DIR)$(TMAIL_ENCODING_DIR) && $(MAKE) $(MAKE_FLAGS) TOPDIR=$(TOPDIR)

$(BUILD_SMTP_TARGET):
	@cd $(SRC_DIR)$(TMAIL_SMTP_DIR) && $(MAKE) $(MAKE_FLAGS) TOPDIR=$(TOPDIR)

$(BUILD_UTILS_TARGET):
	@cd $(SRC_DIR)$(TMAIL_UTILS_DIR) && $(MAKE) $(MAKE_FLAGS) TOPDIR=$(TOPDIR)

$(TEST_TARGET): $(DEFAULT_TARGET)
	@cd $(SRC_DIR)$(TMAIL_UTILS_DIR) && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)
	@cd $(SRC_DIR)$(TMAIL_ENCODING_DIR) && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET)
	@cd $(SRC_DIR)tests && $(MAKE) $(MAKE_FLAGS) $(TEST_TARGET) TOPDIR=$(TOPDIR)

$(INSTALL_TARGET): $(DEFAULT_TARGET)
	@echo "Installing tmail..."
	@$(INSTALL) $(TMAIL_EXECUTABLE) $(BIN_DIR)
	@echo "Installing tmail headers..."
	@$(CP) -r $(TMAIL_HEADERS_DIR) $(HEADERS_DIR)
	@echo "Installing man pages..."
	@$(INSTALL) $(MAN_PAGES_1) $(MAN_DIR_1)
	@$(INSTALL) $(MAN_PAGES_5) $(MAN_DIR_5)
	@echo "Done."

$(UNINSTALL_TARGET):
	@echo "Uninstalling tmail..."
	@rm -rf $(BIN_DIR)/$(TMAIL_EXECUTABLE)
	@echo "Uninstalling man pages..."
	@rm -rf $(MAN_DIR_1)/tmail*
	@rm -rf $(MAN_DIR_5)/tmail*
	@echo "Uninstalling tmail headers..."
	@rm -rf $(HEADERS_DIR)/tmail
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
	@echo  'Cleaning targets:'
	@echo  '  * clean     - Remove most generated files like object files, executables and etc.'
	@echo  ''
	@echo  'Miscellaneous targets'
	@echo  '  * help      - print this output.'

.PHONY: $(CLEAN_TARGET) $(HELP_TARGET) $(INSTALL_TARGET) $(UNINSTALL_TARGET)
