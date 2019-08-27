# tmail core tests
#
# Copyright (C) 2017-2019 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

include ../../mk/Common.mk
include ../../mk/Build.mk

TEST_OBJS += mime_test.o
TEST_OBJS += config_test.o

TEST_EXECUTABLES += mime_test
TEST_EXECUTABLES += config_test

INCLUDE_DIR += -I../ -I../../include

# object files for smtp lib
ifndef SSL_DISABLED
SMTP_OBJS += ../libsmtp/starttls.o
endif
SMTP_OBJS += ../libsmtp/attachment.o
SMTP_OBJS += ../libsmtp/help.o
SMTP_OBJS += ../libsmtp/ehlo.o
SMTP_OBJS += ../libsmtp/smtpauth.o
SMTP_OBJS += ../libsmtp/mailfrom.o
SMTP_OBJS += ../libsmtp/rcpts.o
SMTP_OBJS += ../libsmtp/data.o
SMTP_OBJS += ../libsmtp/body.o
SMTP_OBJS += ../libsmtp/quit.o
SMTP_OBJS += ../libsmtp/smtp.o

ifndef SSL_DISABLED
LIBS += -lssl -lcrypto
else
LIBS +=
endif

# object files for sys lib
SYS_OBJS += ../libsys/at_exit.o
SYS_OBJS += ../libsys/connect.o
SYS_OBJS += ../libsys/fcntl_utils.o
SYS_OBJS += ../libsys/gethostname.o
SYS_OBJS += ../libsys/inet.o

# object files for utils lib
UTILS_OBJS += ../libutils/list.o
UTILS_OBJS += ../libutils/stack.o
UTILS_OBJS += ../libutils/hashmap.o
UTILS_OBJS += ../libutils/string_utils.o

# object files for encoding lib
ENCODING_OBJS += ../libencoding/base64.o

# additional object files that are used for the config_test test
CONFIG_TEST_OBJS += $(ENCODING_OBJS) $(SYS_OBJS) $(UTILS_OBJS) $(SMTP_OBJS)
CONFIG_TEST_OBJS += ../config_parser.tab.o ../lex.yy.o ../config.o ../mime.o config_test.o

TEST_FLAGS = -g $(OSX_FLAGS) $(INCLUDE_DIR) $(DEBUG_FLAGS)

$(TEST_TARGET): $(TEST_EXECUTABLES)
	@exec ./run_tests.pl
	@rm -rf $(TEST_EXECUTABLES)

mime_test:
	$(TMAIL_CC) $(TEST_FLAGS) -c mime_test.c
	$(TMAIL_CC) $(TEST_FLAGS) ../mime.o mime_test.o -o mime_test

config_test:
	$(TMAIL_CC) $(TEST_FLAGS) -c config_test.c
	$(TMAIL_CC) $(TEST_FLAGS) $(CONFIG_TEST_OBJS) -o config_test $(LIBS)

$(CLEAN_TARGET):
	@echo "./src/tests"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(TEST_OBJS)
	@rm -rf $(TEST_EXECUTABLES)

.PHONY: $(CLEAN_TARGET)
