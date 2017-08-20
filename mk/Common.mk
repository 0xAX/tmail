# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Name of main executable
TMAIL_EXECUTABLE=tmail

#
# apps executables
#
SMTP_CAPS_EXECUTABLE = smtp-caps

# Auxiliary sys library
TMAIL_SYS_LIB=libtmail-sys.so
# Auxiliary library with C utils
TMAIL_UTILS_LIB=libtmail-utils.so
# SMTP library
TMAIL_SMTP_LIB=libtmail-smtp.so
# Library that provides different encoding facilities
TMAIL_ENCODING_LIB=libtmail-encoding.so

# Default target to build tmail fully
DEFAULT_TARGET=all
# Main target that will be used to clean everything after build
CLEAN_TARGET=clean
# A target to print a help to stdout
HELP_TARGET=help
# A target to run tests
TEST_TARGET=test
# A target to intall tmail with tools & libs
INSTALL_TARGET=install
# A target to remove tmail from the system
UNINSTALL_TARGET=uninstall
# target to build libtmail-sys.so
BUILD_SYS_LIB_TARGET = libtmail_sys
# target to build libtmail-smtp.so
BUILD_SMTP_LIB_TARGET = libtmail_smtp
# target to build libtmail-utils.so
BUILD_UTILS_LIB_TARGET = libtmail_utils
# target to build libtmail-encoding.so
BUILD_ENCODING_LIB_TARGET = libtmail_encoding
# Build tmail's shared libraries
BUILD_LIBS_TARGET = libs

# Match all object files
OBJECT_FILES=*.o
# Match all shared libraries files
SHARED_LIBS_FILES=*.so

# Main source directory
SRC_DIR=src/
# Directory with makefile auxiliary scripts
MK_DIR=mk/
# Directory with system library
TMAIL_SYS_LIB_DIR=libsys
# Directory with utils library
TMAIL_UTILS_LIB_DIR=libutils
# Directory with smtp library
TMAIL_SMTP_LIB_DIR=libsmtp
# Directory with encoding library
TMAIL_ENCODING_LIB_DIR=libencoding

# Directory to install tmail(s) executables
BIN_DIR ?= /usr/local/bin
# Directory to install tmail(s) libraries
LIB_DIR ?= /usr/lib

# use it for cleanups
RM ?= rm -rf
# default archiver
AR ?= ar
# default linker
LD ?= ld
# default assembler
AS ?= as
# use this one for installing tmail and libraries
INSTALL ?= install
# use this one to strip binaries and libraries
STRIP ?= strip
# default parser generator
BISON ?= bison

# Make flags
# Supress Entering/Leaving messages
SILENT=-s
# Eliminate use of the built-in variables.
ELIMINATE_BUILTIN_VARS=-R
# Eliminate use of the built-in implicit rules.
ELIMINATE_BUILTIN_RULES=-r

# set verbosity
ifndef V
	QUIET_CC		= @echo '   ' CC $@;
	QUIET_CLEAN_OBJECTS	= @echo '   ' RM $(OBJECT_FILES);
	QUIET_CLEAN_TMAIL_EXEC	= @echo '   ' RM $(TMAIL_EXECUTABLE);
	QUIET_CLEAN_SHARED_LIBS = @echo '   ' RM $(SHARED_LIBS_FILES);
	MAKE_FLAGS=$(SILENT) $(ELIMINATE_BUILTINS) $(ELIMINATE_BUILTIN_RULES)
else
	QUIET_CCS=
	QUIET_CLEAN_OBJECTS=
	QUIET_CLEAN_TMAIL_EXEC=
	QUIET_CLEAN_SHARED_LIBS=
	MAKE_FLAGS=
endif

#
# Tests
#
LIBCONF_TEST_EXEC=config_test
