# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Default standard that will be used to build tmail
STANDARD=-std=c11

# Default set of compiler warnings related flags
WARNINGS=-Wextra -Wpedantic -pedantic-errors

# Optimization flags
OPTIMIZATION=-O2

# Debug flags
DEBUG_FLAGS=-g

ifndef DEBUG
MISC_FLAGS=$(OPTIMIZATION)
else
MISC_FLAGS=$(DEBUG_FLAGS)
endif

# Compile object files
COMPILE_FLAGS=-c -o

# Set of compiler flags that will be passed during build in end up
CFLAGS=$(WARNINGS) $(STANDARD) $(MISC_FLAGS) $(COMPILE_FLAGS)

# Set the default C compiler to use
ifndef TMAIL_CC
	TMAIL_CC = gcc
endif
