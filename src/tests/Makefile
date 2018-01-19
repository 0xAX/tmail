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

# additional object files that are used for tests
PARSER_OBJS = ../config_parser.tab.o ../lex.yy.o ../config.o config_test.o

# libraries that are used for tests
LIBS = -ltmail-smtp -lssl -lcrypto -ltmail-sys -ltmail-utils
LIB_DIR = -L../libsmtp/ -L../libsys -L../libutils -L/home/alex/dev/tmail/src/libsmtp

TEST_FLAGS = $(LIB_DIR) $(OSX_FLAGS) $(INCLUDE_DIR) $(DEBUG_FLAGS)

$(TEST_TARGET): $(TEST_EXECUTABLES)
	@exec ./run_tests.pl
	@rm -rf $(TEST_EXECUTABLES)

mime_test:
	$(TMAIL_CC) $(TEST_FLAGS) -c mime_test.c
	$(TMAIL_CC) $(TEST_FLAGS) ../mime.o mime_test.o -o mime_test

config_test:
	$(TMAIL_CC) $(TEST_FLAGS) $(LIBS) -c config_test.c
	$(TMAIL_CC) -Wl,-rpath,$(TOPDIR)/src/libsmtp \
		-Wl,-rpath,$(TOPDIR)/src/libsys \
		-Wl,-rpath,$(TOPDIR)/src/libutils \
		$(TEST_FLAGS) $(PARSER_OBJS) -o config_test $(LIBS)

$(CLEAN_TARGET):
	@echo "./src/tests"
	$(QUIET_CLEAN_OBJECTS) $(RM) $(TEST_OBJS)
	@rm -rf $(TEST_EXECUTABLES)

.PHONY: $(CLEAN_TARGET)
