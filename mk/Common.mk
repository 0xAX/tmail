# tmail Makefile
#
# Copyright (C) 2017-2019 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Name of main executable
TMAIL_EXECUTABLE=tmail

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

# Build tmail's shared libraries
BUILD_SUB_TARGET = sub
# target to build libtmail-sys.so
BUILD_SYS_TARGET = libtmail_sys
# target to build libtmail-smtp.so
BUILD_SMTP_TARGET = libtmail_smtp
# target to build libtmail-utils.so
BUILD_UTILS_TARGET = libtmail_utils
# target to build libtmail-encoding.so
BUILD_ENCODING_TARGET = libtmail_encoding

#
# Documentation related paths
#
MAN_PAGES_1 += Documentation/man/tmail.1
MAN_PAGES_1 += Documentation/man/tmail-smtp-caps.1
MAN_PAGES_1 += Documentation/man/tmail-send-email.1
MAN_PAGES_5 += Documentation/man/tmail-config.5

# Match all object files
OBJECT_FILES=*.o

# Main source directory
SRC_DIR=src/
# tmail headers path
TMAIL_HEADERS_DIR=./include/tmail
# Directory with makefile auxiliary scripts
MK_DIR=mk/
# Directory with system library
TMAIL_SYS_DIR=libsys
# Directory with utils library
TMAIL_UTILS_DIR=libutils
# Directory with smtp library
TMAIL_SMTP_DIR=libsmtp
# Directory with encoding library
TMAIL_ENCODING_DIR=libencoding

# Directory to install tmail(s) executables
BIN_DIR ?= /usr/local/bin
# Path to man dir for programs
MAN_DIR_1 ?= /usr/share/man/man1
# Path to man dir for configuration files
MAN_DIR_5 ?= /usr/share/man/man5
# path to install tmail headers
HEADERS_DIR ?= /usr/include

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
# default lexer
FLEX ?= flex
# default copy command
CP ?= cp

# Make flags
# Supress Entering/Leaving messages
SILENT=-s
# Eliminate use of the built-in variables.
ELIMINATE_BUILTIN_VARS=-R
# Eliminate use of the built-in implicit rules.
ELIMINATE_BUILTIN_RULES=-r

# set verbosity
ifndef V
	QUIET_CC		   = @echo '   ' CC $@;
	QUIET_CLEAN_OBJECTS	   = @echo '   ' RM $(OBJECT_FILES);
	QUIET_CLEAN_TMAIL_EXEC	   = @echo '   ' RM $(TMAIL_EXECUTABLE);
	MAKE_FLAGS=$(SILENT) $(ELIMINATE_BUILTINS) $(ELIMINATE_BUILTIN_RULES)
else
	QUIET_CCS=
	QUIET_CLEAN_OBJECTS=
	QUIET_CLEAN_TMAIL_EXEC=
	MAKE_FLAGS=
endif
