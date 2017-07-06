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
# SMTP library
TMAIL_SMTP_LIB=libtmail-smtp.so

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

# Match all object files
OBJECT_FILES=*.o
# Match all shared libraries files
SHARED_LIBS_FILES=*.so

# Main source directory
SRC_DIR=src/
# Directory with makefile auxiliary scripts
MK_DIR=mk/
# Directory with system library
SYS_DIR_NAME=libsys
# Directory with utils library
UTILS_DIR_NAME=libutils
# Directory with smtp library
TMAIL_SMTP_DIR=libsmtp

# Directory to install tmail(s) executables
BIN_DIR=/usr/local/bin
# Directory to install tmail(s) libraries
LIB_DIR=/usr/lib

# use it for cleanups
RM=rm -rf
# default archiver
AR=ar
# default linker
LD ?= ld
# default assembler
AS ?= as
# use this one for installing tmail and libraries
INSTALL ?= install
# use this one to strip binaries and libraries
STRIP ?= strip

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
