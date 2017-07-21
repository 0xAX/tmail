# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include $(TOPDIR)/mk/Common.mk
include $(TOPDIR)/mk/Build.mk

# apps object files
SMTP_CAPS_APP_OBJ := smtp-caps.o

# apps source code
SMTP_CAPS_SOURCE := smtp-caps.c

# set of directories with C headers needed for tmail
INCLUDES += -I$(TOPDIR)/$(SRC_DIR)libsys/
INCLUDES += -I$(TOPDIR)/$(SRC_DIR)libutils/                    
INCLUDES += -I$(TOPDIR)/$(SRC_DIR)libsmtp/
INCLUDES += -I./

# libraries which are required by tmail
LD_SYS_LIB := -ltmail-sys
LD_SMTP_LIB := -ltmail-smtp
LD_UTILS_LIB := -ltmail-utils

# paths to these libraries
# (we should add this at least to compile tmail
#  without installation of libraries to system)
LD_SYS_LIB_PATH := -L$(TOPDIR)/$(SRC_DIR)libsys
LD_SMTP_LIB_PATH := -L$(TOPDIR)/$(SRC_DIR)libsmtp
LD_UTILS_LIB_PATH := -L$(TOPDIR)/$(SRC_DIR)libutils

# Tell to linker where libraries are
LINKER_OPTS+=-Wl,-rpath,$(TOPDIR)/$(SRC_DIR)libsys
LINKER_OPTS+=-Wl,-rpath,$(TOPDIR)/$(SRC_DIR)libutils
LINKER_OPTS+=-Wl,-rpath,$(TOPDIR)/$(SRC_DIR)libsmtp

COMPILER_FLAGS = $(INCLUDES) $(LD_SMTP_LIB_PATH) $(LINKER_OPTS) $(CC_EXEC_FLAGS)

$(DEFAULT_TARGET): $(SMTP_CAPS_EXECUTABLE)

$(SMTP_CAPS_EXECUTABLE): $(SMTP_CAPS_APP_OBJ)
	$(QUIET_CC) $(TMAIL_CC)	$(COMPILER_FLAGS) $(SMTP_CAPS_EXECUTABLE) $(SMTP_CAPS_APP_OBJ) $(LD_SMTP_LIB)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src/apps"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@echo "    RM " $(SMTP_CAPS_EXECUTABLE)
	$(RM) $(SMTP_CAPS_EXECUTABLE)

.PHONY: $(CLEAN)
