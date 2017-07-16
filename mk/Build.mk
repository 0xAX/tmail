# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Default standard that will be used to build tmail
STANDARD=-std=gnu11

# an architecture flags
ifndef NON_NATIVE_BINARY
ARCH=-march=native -mtune=native -ftree-vectorize
else
ARCH=-ftree-vectorize
endif

# Default set of compiler warnings related flags
WARNINGS+=-Wall
WARNINGS+=-Werror
WARNINGS+=-Wextra
WARNINGS+=-Wshadow
WARNINGS+=-Wfatal-errors
WARNINGS+=-pedantic-errors
WARNINGS+=-Wformat
WARNINGS+=-Wformat-security
WARNINGS+=-Werror=format-security

# Optimization flags
OPTIMIZATION+=-O2
OPTIMIZATION+=-D_FORTIFY_SOURCE=2

# Debug flags
DEBUG_FLAGS+=-O
DEBUG_FLAGS+=-g3
DEBUG_FLAGS+=-fstack-protector

# Set the default C compiler to use
TMAIL_CC ?= gcc

# Add compiler related warnings flags
ifeq ($(TMAIL_CC), gcc)
WARNINGS += -fmax-errors=2
else ifeq ($(TMAIL_CC), clang)
WARNINGS += -ferror-limit=2
endif

# Compile with debug info or optimization stuff
# depends on `DEBUG` flags
ifndef DEBUG
MISC_FLAGS=$(OPTIMIZATION)
else
MISC_FLAGS=$(DEBUG_FLAGS)
endif

# Compile a shared library
CC_SHLIB_FLAGS=-shared -o
# Compile an executable
CC_EXEC_FLAGS=-o

# Set of compiler flags that will be passed during build of executables
CFLAGS_OBJS=$(WARNINGS) $(STANDARD) $(ARCH) $(MISC_FLAGS) -c -o
# Set of compiler flags that will be passed during build of shared libraries
CFLAGS_LIBS=-c $(WARNINGS) $(STANDARD) $(ARCH) $(MISC_FLAGS) -fpic

# allow to use gnu extensions in clang 4.0.0 and above
ifeq ($(TMAIL_CC), clang)
CFLAGS_LIBS += -Wno-gnu
endif
