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
BUILD_ROOT ?= $(SRC_ROOT)/build

KERNEL ?= $(BUILD_ROOT)/tupai.elf

KSYMBOLS ?= $(BUILD_ROOT)/tupai.symb

KNAME               = tupai
KNAME_DECORATIVE    = Tupai
KVERSION_DECORATIVE = 0.6.0
KMAINTAINER_NAME    = Joshua Barretto
KMAINTAINER_EMAIL   = joshua.s.barretto@gmail.com

# CFlags
# -----

INCS += $(SRC_ROOT)/include

INC_FLAGS = $(addprefix -I, $(INCS))

GCCFLAGS  = -DKNAME=\"$(KNAME)\" -DKNAME_DECORATIVE=\"$(KNAME_DECORATIVE)\" -DKVERSION_DECORATIVE=\"$(KVERSION_DECORATIVE)\" -DKMAINTAINER_NAME="\"$(KMAINTAINER_NAME)\"" -DKMAINTAINER_EMAIL=\"$(KMAINTAINER_EMAIL)\"
GCCFLAGS += -DARCH_FAMILY_$(TARGET_FAMILY) -DARCH_TARGET_$(TARGET_ARCH)
GCCFLAGS += $(INC_FLAGS)
GCCFLAGS += -Wall -Wextra -O3 -nostdlib
GCCFLAGS += -ffreestanding -fno-common -fno-builtin -fno-omit-frame-pointer

ifeq ($(TARGET_ARCH), x86_64)
	GCCFLAGS += -DARCH_ADDRESS_64
	GCCFLAGS += -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2
endif
ifeq ($(TARGET_ARCH), i386)
	GCCFLAGS += -DARCH_ADDRESS_32
	GCCFLAGS +=
endif
ifeq ($(TARGET_ARCH), rpi3)
	GCCFLAGS += -DARCH_ADDRESS_32
	GCCFLAGS += -mcpu=arm1176jzf-s
endif

CFLAGS += $(GCCFLAGS)
CFLAGS += -std=c11

CPPFLAGS += $(GCCFLAGS)
CPPFLAGS += -std=c++1z -fno-exceptions -fno-stack-protector -fno-rtti

# LFLAGS
# ------

LFLAGS = -lgcc

ifeq ($(TARGET_ARCH), x86_64)
	LFLAGS += -z max-page-size=4096
	LSCRIPT = $(SRC_DIR)/x86/x86_64/link.ld
endif
ifeq ($(TARGET_ARCH), i386)
	LSCRIPT = $(SRC_DIR)/x86/i386/link.ld
endif
ifeq ($(TARGET_ARCH), rpi3)
	LSCRIPT = $(SRC_DIR)/arm/rpi3/link.ld
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

AS  = $(ARCH_GCC_PREFIX)g++
CC  = $(ARCH_GCC_PREFIX)g++
CPP = $(ARCH_GCC_PREFIX)g++
LD  = $(ARCH_GCC_PREFIX)g++
AR  = $(ARCH_GCC_PREFIX)ar

TOOL_DIR ?= $(abspath $(dir $(shell which $(CC))))

KSYMBOL_GEN = objdump --wide --syms $(KERNEL) | grep -P '^[0-9A-Fa-f]+\s.*\s[a-zA-Z_][a-zA-Z0-9_]+$$' | sed -r 's/^(\S+)\s+.*\s+(\S+)$$/\1 \2/' | sort > $(KSYMBOLS)

# Source files
# ------------

SRC_DIR = $(abspath .)

SRCS_REL += $(shell ls $(SRC_ROOT)/*.{s,S,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls $(SRC_ROOT)/util/*.{s,S,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls $(SRC_ROOT)/sys/*.{s,S,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls $(SRC_ROOT)/dev/*.{s,S,c,cpp} 2> /dev/null)

ifeq ($(TARGET_FAMILY), x86)
	SRCS_REL += $(shell ls $(SRC_ROOT)/x86/*.{s,S,c,cpp} 2> /dev/null)
	SRCS_REL += $(shell ls $(SRC_ROOT)/x86/vga/*.{s,S,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_FAMILY), arm)
	SRCS_REL += $(shell ls $(SRC_ROOT)/arm/*.{s,S,c,cpp} 2> /dev/null)
endif

ifeq ($(TARGET_ARCH), x86_64)
	SRCS_REL += $(shell ls $(SRC_ROOT)/x86/x86_64/*.{s,S,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_ARCH), i386)
	SRCS_REL += $(shell ls $(SRC_ROOT)/x86/i386/*.{s,S,c,cpp} 2> /dev/null)
	SRCS_REL += $(shell ls $(SRC_ROOT)/x86/i386/cpu/*.{s,S,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_ARCH), rpi3)
	SRCS_REL += $(shell ls $(SRC_ROOT)/arm/rpi3/*.{s,S,c,cpp} 2> /dev/null)
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

all : $(KERNEL) $(KSYMBOLS)

clean :
	@rm -r -f $(KERNEL) $(OBJS) $(DEPS) $(BDIRS)

$(KERNEL) : $(OBJS)
	@echo "[`date "+%H:%M:%S"`] Linking '$@'..."
	@$(TOOL_DIR)/$(LD) $(GCCFLAGS) -T $(LSCRIPT) -o $@ $(CRTBEGIN) $(OBJS) $(CRTEND) $(LFLAGS)
	@echo "[`date "+%H:%M:%S"`] Linked '$@'."

$(KSYMBOLS) : $(KERNEL)
	@echo "Generating kernel symbols..."
	@$(KSYMBOL_GEN)
	@echo "Generated kernel symbols"

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
	@$(TOOL_DIR)/$(CPP) -MMD -c -o $@ $< $(CPPFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."
