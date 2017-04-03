# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Current operating system
OS := $(shell uname)

# Name of main executable
TMAIL_EXECUTABLE=tmail
# Main target that will be used to clean everything after build
CLEAN_TARGET=clean
# A target to print a help to stdout
HELP_TARGET=help
# Match all object files
OBJECT_FILES=*.o
# Directory with makefile auxiliary scripts
MK_DIR=mk/

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
endif
