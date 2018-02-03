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
WARNINGS += -Wall -Werror -Wextra -Wshadow -Wfatal-errors -pedantic-errors
WARNINGS += -Wformat -Wformat-security -Werror=format-security
WARNINGS += -Wdeclaration-after-statement -Wswitch-enum -Wswitch-default
WARNINGS += -Wundef -Wbad-function-cast -Wstrict-prototypes -Winline

ifeq ($(TMAIL_CC), gcc)
$(call gcc-supports-flag,UNUSED_CONST_VAR_WARN,-Wunused-const-variable)
$(call gcc-supports-flag,EXPANSION_TO_DEFINED_WARN,-Wexpansion-to-defined)
$(call gcc-supports-flag,SIZOF_ARRAY_WARN,-Wsizeof-array-argument)
$(call gcc-supports-flag,SWITCH_UNREACHEABLE_WARN,-Wswitch-unreachable)
$(call gcc-supports-flag,DUPLICATED_BRANCES_WARN,-Wduplicated-branches)
$(call gcc-supports-flag,POINTER_COMPARE_WARN,-Wpointer-compare)
else ifeq ($(TMAIL_CC), clang)
$(call clang-supports-flag,UNUSED_CONST_VAR_WARN,-Wunused-const-variable)
$(call clang-supports-flag,EXPANSION_TO_DEFINED_WARN,-Wexpansion-to-defined)
$(call clang-supports-flag,SIZOF_ARRAY_WARN,-Wsizeof-array-argument)
$(call clang-supports-flag,SWITCH_UNREACHEABLE_WARN,-Wswitch-unreachable)
$(call clang-supports-flag,DUPLICATED_BRANCES_WARN,-Wduplicated-branches)
$(call clang-supports-flag,POINTER_COMPARE_WARN,-Wpointer-compare)
endif

ifeq "$(DUPLICATED_BRANCES_WARN)" "1"
WARNINGS+=-Wduplicated-branches
endif

ifeq "$(POINTER_COMPARE_WARN)" "1"
WARNINGS+=-Wpointer-compare
endif

ifeq "$(SWITCH_UNREACHEABLE_WARN)" "1"
WARNINGS+=-Wswitch-unreachable
endif

ifeq "$(SIZOF_ARRAY_WARN)" "1"
WARNINGS+=-Wsizeof-array-argument
endif

ifeq "$(UNUSED_CONST_VAR_WARN)" "1"
	WARNINGS+=-Wunused-const-variable
endif

ifeq "$(EXPANSION_TO_DEFINED_WARN)" "1"
	WARNINGS+=-Wexpansion-to-defined
endif

# Optimization flags
OPTIMIZATION += -O2

# Debug flags
DEBUG_FLAGS += -O -g3 -ggdb -D_FORTIFY_SOURCE=2

# Add compiler related warnings flags
ifeq ($(findstring gcc, $(TMAIL_CC)), gcc)
WARNINGS += -fmax-errors=2 -Wswitch-bool -Wlogical-op
WARNINGS += -Wsuggest-attribute=noreturn -Wsuggest-final-types
WARNINGS += -Wduplicated-cond
else ifeq ($(findstring clang, $(TMAIL_CC)), clang)
WARNINGS += -ferror-limit=2 -Qunused-arguments
endif

# Compile with debug info or optimization stuff
# depends on `DEBUG` flags
ifndef DEBUG
MISC_FLAGS=$(OPTIMIZATION)
else
MISC_FLAGS=$(DEBUG_FLAGS)
WARNINGS =
endif

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

#
# Definitions that will be passed to compiler
#
DEFS += -DTMAIL_VERSION=\"0.0.1\"

ifndef SSL_DISABLED
	OPENSSL_VERSION=$(shell openssl version 2>/dev/null | sed -E -e "s/(OpenSSL|LibreSSL).//" | sed -e "s/\.[0-9][a-z].*//")

	ifeq ($(OPENSSL_VERSION), 1.1)
		DEFS += -DSSL_V11=\"1\"
	endif

	ifeq ($(OPENSSL_VERSION), 1.0)
		DEFS += -DSSL_V10=\"1\"
	endif

	ifeq ($(OPENSSL_VERSION), 2.6)
		DEFS += -DSSL_V11=\"1\"
	endif
else
	SSL_DISABLED = 1
	DEFS += -DSSL_DISABLED=\"1\"
endif

#
# OS related stuff
#
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	ifndef SSL_DISABLED
		OSX_FLAGS=-I/usr/local/opt/openssl/include
	endif
endif
