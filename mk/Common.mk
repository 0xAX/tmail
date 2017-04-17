# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Name of main executable
TMAIL_EXECUTABLE=tmail
# Auxiliary sys library
TMAIL_SYS_LIB=libtmail-sys.so
# Auxiliary library with C utils
TMAIL_UTILS_LIB=libtmail-utils.so

# Main target that will be used to clean everything after build
CLEAN_TARGET=clean
# A target to print a help to stdout
HELP_TARGET=help
# A target to run tests
TEST_TARGET=test

# Match all object files
OBJECT_FILES=*.o
# Match all shared libraries files
SHARED_LIBS_FILES=*.so

# Main source directory
SRC_DIR=src/
# Directory with makefile auxiliary scripts
MK_DIR=mk/
# Directory with system library
SYS_DIR_NAME=sys
# Directory with utils library
UTILS_DIR_NAME=libutils

# use it for cleanups
RM=rm -rf
# default archiver
AR=ar
# default linker
LD ?= ld
# default assembler
AS ?= as

# set verbosity
ifndef V
	QUIET_CC		= @echo '   ' CC $@;
	QUIET_CLEAN_OBJECTS	= @echo '   ' RM $(OBJECT_FILES);
	QUIET_CLEAN_EXECS	= @echo '   ' RM $(TMAIL_EXECUTABLE);
	QUIET_CLEAN_SHARED_LIBS = @echo '   ' RM $(SHARED_LIBS_FILES);
        MAKE_FLAGS=-s
else
	QUIET_CCS=
	QUIET_CLEAN_OBJECTS=
	QUIET_CLEAN_EXECS=
	QUIET_CLEAN_SHARED_LIBS=
        QUIET_CC_TESTS=
	MAKE_FLAGS=
endif

#
# Tests
#
LIBCONF_TEST_EXEC=config_test
