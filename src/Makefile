# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

.DEFAULT_GOAL: $(TMAIL_EXECUTABLE)

include ../mk/Common.mk
include ../mk/Build.mk

# object files for
TMAIL_OBJS += tmail.o
TMAIL_OBJS += message.o
TMAIL_OBJS += send-email.o

# object files for sys lib
LIBSYS_OBJS += libsys/at_exit.o
LIBSYS_OBJS += libsys/connect.o
LIBSYS_OBJS += libsys/fcntl_utils.o
LIBSYS_OBJS += libsys/gethostname.o
LIBSYS_OBJS += libsys/inet.o

# object files for utils lib
LIB_UTILS_OBJS += libutils/list.o
LIB_UTILS_OBJS += libutils/stack.o

# object files for smtp lib
LIB_SMTP_OBJS += libsmtp/ehlo.o
LIB_SMTP_OBJS += libsmtp/mail_from.o
LIB_SMTP_OBJS += libsmtp/rcpt_to.o
LIB_SMTP_OBJS += libsmtp/data_msg.o
LIB_SMTP_OBJS += libsmtp/body.o
LIB_SMTP_OBJS += libsmtp/quit.o
LIB_SMTP_OBJS += libsmtp/smtp.o

# other apps
SMTP_CAPS_OBJS += smtp-caps.o

# set of directories with C headers needed for tmail
INCLUDES+=-Ilibsys/
INCLUDES+=-Ilibutils/                    
INCLUDES+=-Ilibsmtp/
INCLUDES+=-I./

COMPILER_FLAGS = $(LIBS_PATH) $(INCLUDES) $(LINKER_OPTS) $(CC_EXEC_FLAGS)

$(TMAIL_EXECUTABLE): $(TMAIL_OBJS)
	$(QUIET_CC) $(TMAIL_CC)	$(COMPILER_FLAGS) ../$@	$(LIBSYS_OBJS) $(LIB_UTILS_OBJS) $(LIB_SMTP_OBJS) $(TMAIL_OBJS) $(LIBS)

$(SMTP_CAPS_EXECUTABLE): $(SMTP_CAPS_OBJS)
	$(QUIET_CC) $(TMAIL_CC)	$(COMPILER_FLAGS) ../$@ $(LIBSYS_OBJS) $(LIB_UTILS_OBJS) $(LIB_SMTP_OBJS) $(SMTP_CAPS_OBJS) $(LIBS)

%.o: %.c
	$(QUIET_CC) $(TMAIL_CC) $(INCLUDES) $(CFLAGS_OBJS) $@ $<

$(CLEAN_TARGET):
	@echo "./src"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(OBJECT_FILES)
	@cd $(TMAIL_SYS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)
	@cd $(TMAIL_UTILS_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)
	@cd $(TMAIL_SMTP_LIB_DIR) && $(MAKE) $(MAKE_FLAGS) $(CLEAN_TARGET)

.PHONY: $(CLEAN_TARGET)
