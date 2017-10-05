#
# file : makefile
#
# Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
#
# This file is part of Tupai.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

# Definitions
# -----------

TARGET_FAMILY ?= x86
TARGET_ARCH   ?= i386

SRC_ROOT    = $(abspath .)

ARCHIVE ?= $(BUILD_ROOT)/libc.a

ARCHIVE_SYSROOT ?= $(SYSROOT_BUILD_DIR)/lib/$(notdir $(ARCHIVE))
ARCHIVE_SYSROOT_PATH = $(dir $(ARCHIVE_SYSROOT))

# CFlags
# -----

INCS += $(SYSROOT_SRC_DIR)/include $(SRC_ROOT)/include

INC_FLAGS = $(addprefix -I, $(INCS))

CFLAGS += -DARCH_FAMILY_$(TARGET_FAMILY) -DARCH_TARGET_$(TARGET_ARCH)
CFLAGS += $(INC_FLAGS)
CFLAGS += -std=gnu11 -Wall -Wextra -O3 -nostdlib
CFLAGS += -ffreestanding -fno-exceptions -fno-common -fno-builtin -fno-omit-frame-pointer # TODO : Get rid of -ffreestanding and add a proper libc

ifeq ($(TARGET_ARCH), x86_64)
	CFLAGS += -DARCH_ADDRESS_64
	CFLAGS += -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2
endif
ifeq ($(TARGET_ARCH), i386)
	CFLAGS += -DARCH_ADDRESS_32
	CFLAGS +=
endif
ifeq ($(TARGET_ARCH), rpi3)
	CFLAGS += -DARCH_ADDRESS_32
	CFLAGS += -mcpu=arm1176jzf-s
endif

# Tools
# -----

ifeq ($(TARGET_ARCH), x86_64)
	ARCH_GCC_PREFIX = x86_64-elf-
endif
ifeq ($(TARGET_ARCH), i386)
	ARCH_GCC_PREFIX = i686-elf-
endif
ifeq ($(TARGET_ARCH), rpi3)
	ARCH_GCC_PREFIX = arm-none-eabi-
endif

AS  = $(ARCH_GCC_PREFIX)gcc
CC  = $(ARCH_GCC_PREFIX)gcc
CPP = $(ARCH_GCC_PREFIX)gcc
LD  = $(ARCH_GCC_PREFIX)gcc
AR  = $(ARCH_GCC_PREFIX)ar

TOOL_DIR ?= $(abspath $(dir $(shell which $(CC))))

# Source files
# ------------

SRC_DIR = $(abspath .)

SRCS_REL += $(shell ls $(SRC_ROOT)/*.{s,S,c,cpp} 2> /dev/null)

ifeq ($(TARGET_FAMILY), x86)
	SRCS_REL += $(shell ls $(SRC_ROOT)/arch/x86/*.{s,S,c,cpp} 2> /dev/null)
endif

ifeq ($(TARGET_ARCH), x86_64)
	SRCS_REL += $(shell ls $(SRC_ROOT)/arch/x86/x86_64/*.{s,S,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_ARCH), i386)
	SRCS_REL += $(shell ls $(SRC_ROOT)/arch/x86/i386/*.{s,S,c,cpp} 2> /dev/null)
endif

SRCS = $(abspath $(SRCS_REL))

OBJS  += $(subst $(SRC_ROOT), $(BUILD_ROOT), $(addsuffix .o, $(SRCS)))
DEPS  += $(subst $(SRC_ROOT), $(BUILD_ROOT), $(addsuffix .d, $(SRCS)))
BDIRS += $(sort $(dir $(OBJS)))

CRTBEGIN = $(shell $(LD) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND   = $(shell $(LD) $(CFLAGS) -print-file-name=crtend.o)

# Rules
# -----

.PHONY : all clean

all : $(ARCHIVE) $(ARCHIVE_SYSROOT)

clean :
	@rm -r -f $(ARCHIVE) $(OBJS) $(DEPS) $(BDIRS)

$(ARCHIVE_SYSROOT) : $(ARCHIVE)
	@mkdir -p $(ARCHIVE_SYSROOT_PATH)
	@cp $(ARCHIVE) $(ARCHIVE_SYSROOT)

$(ARCHIVE) : $(OBJS)
	@echo "[`date "+%H:%M:%S"`] Creating '$@'..."
	@$(TOOL_DIR)/$(AR) rcs -o $@ $(CRTBEGIN) $(OBJS) $(CRTEND)
	@echo "[`date "+%H:%M:%S"`] Created '$@'."

$(OBJS) : | $(BDIRS)

$(BDIRS) :
	@mkdir -p $(BDIRS)
	@echo "Created build directory '$@'."

-include $(DEPS)

# Compile .s files
$(BUILD_ROOT)/%.s.o : $(SRC_DIR)/%.s
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(AS) -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."

# Compile .S files
$(BUILD_ROOT)/%.S.o : $(SRC_DIR)/%.S
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(AS) -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."

# Compile .c files
$(BUILD_ROOT)/%.c.o : $(SRC_DIR)/%.c
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(CC) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."

# Compile .cpp files
$(BUILD_ROOT)/%.cpp.o : $(SRC_DIR)/%.cpp
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(CPP) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."
