# tmail Makefile
#
# Copyright (C) 2017 Alexander Kuleshov <kuleshovmail at gmail dot com>
# This file is released under the BSD license, see the COPYING file

# Default standard that will be used to build tmail
STANDARD=-std=gnu11

# Set the default C compiler to use
TMAIL_CC ?= gcc

#
# some useful routines
#
define gcc-supports-flag
	$(eval $1 = $(shell echo "" | gcc $2 2>&1 | grep -q "unrecognized command"; echo "$$?"))
endef

define clang-supports-flag
	$(eval $1 = $(shell echo "" | clang -E -c -Wfd - 2>&1 | grep -q unknown; echo "$$?"))
endef

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
WARNINGS+=-Wswitch-enum
WARNINGS+=-Wswitch-default
WARNINGS+=-Wdeclaration-after-statement
WARNINGS+=-Wshadow
WARNINGS+=-Wundef
WARNINGS+=-Wbad-function-cast
WARNINGS+=-Wsizeof-array-argument
WARNINGS+=-Wstrict-prototypes
WARNINGS+=-Winline

ifeq ($(TMAIL_CC), gcc)
$(call gcc-supports-flag,UNUSED_CONST_VAR_WARN,-Wunused-const-variable)
$(call gcc-supports-flag,EXPANSION_TO_DEFINED_WARN,-Wexpansion-to-defined)
else ifeq ($(TMAIL_CC), clang)
$(call clang-supports-flag,UNUSED_CONST_VAR_WARN,-Wunused-const-variable)
$(call clang-supports-flag,EXPANSION_TO_DEFINED_WARN,-Wexpansion-to-defined)
endif

ifeq "$(UNUSED_CONST_VAR_WARN)" "1"
	WARNINGS+=-Wunused-const-variable
endif

ifeq "$(EXPANSION_TO_DEFINED_WARN)" "1"
	WARNINGS+=-Wexpansion-to-defined
endif

# Optimization flags
OPTIMIZATION+=-O2
OPTIMIZATION+=-D_FORTIFY_SOURCE=2

# Debug flags
DEBUG_FLAGS+=-O
DEBUG_FLAGS+=-g3
DEBUG_FLAGS+=-fstack-protector

# Add compiler related warnings flags
ifeq ($(TMAIL_CC), gcc)
WARNINGS += -fmax-errors=2
WARNINGS+=-Wswitch-bool
WARNINGS+=-Wlogical-op
WARNINGS+=-Wswitch-unreachable
WARNINGS+=-Wsuggest-attribute=pure
WARNINGS+=-Wsuggest-attribute=const
WARNINGS+=-Wsuggest-attribute=noreturn
WARNINGS+=-Wsuggest-final-types
WARNINGS+=-Wduplicated-branches
WARNINGS+=-Wduplicated-cond
WARNINGS+=-Wpointer-compare
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

# allow to use gnu extensions in clang 4.0.0 and above
ifeq ($(TMAIL_CC), clang)
CFLAGS_LIBS += -Wno-gnu
CFLAGS_OBJS += -Wno-gnu
endif

# Set of compiler flags that will be passed during build of executables
CFLAGS_OBJS += $(WARNINGS) $(STANDARD) $(ARCH) $(MISC_FLAGS) -c -o
# Set of compiler flags that will be passed during build of shared libraries
CFLAGS_LIBS += -c $(WARNINGS) $(STANDARD) $(ARCH) $(MISC_FLAGS) -fpic
