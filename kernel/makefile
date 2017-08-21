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

TARGET_FAMILY ?= X86
TARGET_ARCH   ?= AMD64

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

INCS += $(LIBC_INC) $(SRC_ROOT)/include

INC_FLAGS = $(addprefix -I, $(INCS))

CFLAGS  = -DKNAME=\"$(KNAME)\" -DKNAME_DECORATIVE=\"$(KNAME_DECORATIVE)\" -DKVERSION_DECORATIVE=\"$(KVERSION_DECORATIVE)\" -DKMAINTAINER_NAME="\"$(KMAINTAINER_NAME)\"" -DKMAINTAINER_EMAIL=\"$(KMAINTAINER_EMAIL)\"
CFLAGS += -DARCH_FAMILY_$(TARGET_FAMILY)
CFLAGS += $(INC_FLAGS)
CFLAGS += -std=c++14 -Wall -Wextra -pedantic -gdwarf-4 -O3 -nostdlib
CFLAGS += -ffreestanding -fno-exceptions -fno-rtti -fno-common -fno-builtin -fno-omit-frame-pointer

ifeq ($(TARGET_ARCH), AMD64)
	CFLAGS += -DARCH_amd64 -DARCH_ADDRESS_64 -DARCH_FAMILY_x86
	CFLAGS += -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2
endif
ifeq ($(TARGET_ARCH), I386)
	CFLAGS += -DARCH_i386 -DARCH_ADDRESS_32 -DARCH_FAMILY_x86
	CFLAGS +=
endif
ifeq ($(TARGET_ARCH), RPI2)
	CFLAGS += -DARCH_rpi2 -DARCH_ADDRESS_32 -DARCH_FAMILY_arm
	CFLAGS += -mcpu=arm1176jzf-s
endif

# LFLAGS
# ------

LFLAGS = -lgcc

ifeq ($(TARGET_ARCH), AMD64)
	LFLAGS += -z max-page-size=4096
	LSCRIPT = $(SRC_DIR)/x86/amd64/linker.ld
endif
ifeq ($(TARGET_ARCH), I386)
	LSCRIPT = $(SRC_DIR)/x86/i386/linker.ld
endif
ifeq ($(TARGET_ARCH), RPI2)
	LSCRIPT = $(SRC_DIR)/arm/rpi2/linker.ld
endif

# Tools
# -----

ifeq ($(TARGET_ARCH), AMD64)
	AS  = x86_64-elf-g++
	CC  = x86_64-elf-g++
	CPP = x86_64-elf-g++
	LD  = x86_64-elf-g++
endif
ifeq ($(TARGET_ARCH), I386)
	AS  = i686-elf-g++
	CC  = i686-elf-g++
	CPP = i686-elf-g++
	LD  = i686-elf-g++
endif
ifeq ($(TARGET_ARCH), RPI2)
	AS  = arm-none-eabi-g++
	CC  = arm-none-eabi-g++
	CPP = arm-none-eabi-g++
	LD  = arm-none-eabi-g++
endif

TOOL_DIR ?= $(abspath $(dir $(shell which $(CC))))

KSYMBOL_GEN = objdump --wide --syms $(KERNEL) | grep -P '^[0-9A-Fa-f]+\s.*\s[a-zA-Z_][a-zA-Z0-9_]+$$' | sed -r 's/^(\S+)\s+.*\s+(\S+)$$/\1 \2/' | sort > $(KSYMBOLS)

# Source files
# ------------

SRC_DIR = $(abspath .)

SRCS_REL += $(shell ls ./*.{s,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls ./dev/*.{s,c,cpp}  2> /dev/null)
SRCS_REL += $(shell ls ./libk/*.{s,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls ./mem/*.{s,c,cpp}  2> /dev/null)
SRCS_REL += $(shell ls ./sys/*.{s,c,cpp}  2> /dev/null)
SRCS_REL += $(shell ls ./task/*.{s,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls ./util/*.{s,c,cpp} 2> /dev/null)
SRCS_REL += $(shell ls ./vfs/*.{s,c,cpp}  2> /dev/null)

ifeq ($(TARGET_FAMILY), X86)
	SRCS_REL += $(shell ls ./x86/*.{s,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_FAMILY), ARM)
	SRCS_REL += $(shell ls ./arm/*.{s,c,cpp} 2> /dev/null)
	SRCS_REL += $(shell ls ./arm/util/*.{s,c,cpp} 2> /dev/null)
endif

ifeq ($(TARGET_ARCH), AMD64)
	SRCS_REL += $(shell ls ./x86/amd64/*.{s,c,cpp}      2> /dev/null)
	SRCS_REL += $(shell ls ./x86/amd64/crt/*.{s,c,cpp}  2> /dev/null)
	SRCS_REL += $(shell ls ./x86/amd64/sys/*.{s,c,cpp}  2> /dev/null)
	SRCS_REL += $(shell ls ./x86/amd64/util/*.{s,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_ARCH), I386)
	SRCS_REL += $(shell ls ./x86/i386/*.{s,c,cpp}      2> /dev/null)
	SRCS_REL += $(shell ls ./x86/i386/crt/*.{s,c,cpp}  2> /dev/null)
	SRCS_REL += $(shell ls ./x86/i386/sys/*.{s,c,cpp}  2> /dev/null)
	SRCS_REL += $(shell ls ./x86/i386/util/*.{s,c,cpp} 2> /dev/null)
endif
ifeq ($(TARGET_ARCH), RPI2)
	SRCS_REL += $(shell ls ./arm/rpi2/*.{s,c,cpp} 2> /dev/null)
endif

SRCS = $(abspath $(SRCS_REL))

OBJS  += $(subst $(SRC_ROOT), $(BUILD_ROOT), $(addsuffix .o, $(basename $(SRCS))))
DEPS  += $(subst $(SRC_ROOT), $(BUILD_ROOT), $(addsuffix .d, $(basename $(SRCS))))
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
	@$(TOOL_DIR)/$(LD) $(CFLAGS) -T $(LSCRIPT) -o $@ $(CRTBEGIN) $(OBJS) $(CRTEND) $(LFLAGS)
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
$(BUILD_ROOT)/%.o : $(SRC_DIR)/%.s
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(AS) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."

# Compile .c files
$(BUILD_ROOT)/%.o : $(SRC_DIR)/%.c
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(CC) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."

# Compile .cpp files
$(BUILD_ROOT)/%.o : $(SRC_DIR)/%.cpp
	@echo "[`date "+%H:%M:%S"`] Compiling '$<'..."
	@$(TOOL_DIR)/$(CPP) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$@'."
