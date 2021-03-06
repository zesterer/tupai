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

# Architecture configuration

# x86 / arm
TARGET_FAMILY = x86
# i386 / x86_64 / rpi3
TARGET_ARCH   = i386

SRC_ROOT    = $(abspath .)
BUILD_ROOT ?= $(SRC_ROOT)/build

INCLUDE_DIR = $(SRC_ROOT)/include

# Sysroot
SYSROOT_SRC_DIR     = $(SRC_ROOT)/sysroot
SYSROOT_BUILD_DIR   = $(BUILD_ROOT)/sysroot
SYSROOT_INCLUDE_DIR = $(SYSROOT_BUILD_DIR)/usr/include

# Kernel
KERNEL_SRC_DIR    = $(SRC_ROOT)/kernel
KERNEL_BUILD_ROOT = $(BUILD_ROOT)/kernel
KERNEL            = $(KERNEL_BUILD_ROOT)/tupai.elf
KERNEL_MAKE_ARGS  = BUILD_ROOT=$(KERNEL_BUILD_ROOT) KERNEL=$(KERNEL) TARGET_FAMILY=$(TARGET_FAMILY) TARGET_ARCH=$(TARGET_ARCH)

# Userland
USERLAND_SRC_DIR   = $(SRC_ROOT)/userland
USERLAND_BUILD_DIR = $(BUILD_ROOT)/userland
USERLAND_MAKE_ARGS = BUILD_DIR=$(USERLAND_BUILD_DIR) SYSROOT_SRC_DIR=$(SYSROOT_SRC_DIR) SYSROOT_BUILD_DIR=$(SYSROOT_BUILD_DIR) TARGET_FAMILY=$(TARGET_FAMILY) TARGET_ARCH=$(TARGET_ARCH)

# Initrd
INITRD              = $(BUILD_ROOT)/initrd.tar
INITRD_DEPS         = $(shell find $(SYSROOT_SRC_DIR) -name '*')

# GRUB
GRUB_BUILD_DIR = $(BUILD_ROOT)/grub
GRUB_SRC_DIR   = grub/

ISO = $(BUILD_ROOT)/tupai.iso

# Tools
# -----

TAR = tar

QEMU_ARGS = -d guest_errors --no-reboot --no-shutdown -m 256M
ifeq ($(TARGET_ARCH), x86_64)
	QEMU = qemu-system-x86_64
	QEMU_ARGS +=
endif
ifeq ($(TARGET_ARCH), i386)
	QEMU = qemu-system-i386
	QEMU_ARGS +=
endif
ifeq ($(TARGET_ARCH), rpi3)
	QEMU = qemu-system-arm
	QEMU_ARGS += -M raspi3
endif

BOCHS = bochs

# Rules
# -----

.PHONY : all build rebuild kernel userland iso clean run debug

all : build
build : iso
rebuild : clean iso
iso : $(ISO)

clean :
	@cd $(KERNEL_SRC_DIR) && $(MAKE) clean $(KERNEL_MAKE_ARGS)
	@rm -r -f $(ISO) $(GRUB_BUILD_DIR) $(INITRD) $(BUILD_ROOT)
	@echo "Cleaned all."

run : build
	@echo "Running '$(ISO)'..."
	@$(QEMU) $(QEMU_ARGS) -cdrom $(ISO)

debug : build
	@echo "Debugging '$(ISO)'..."
	@$(BOCHS)

$(ISO) : $(KERNEL) $(INITRD)
	@mkdir -p $(GRUB_BUILD_DIR)/isodir/boot/grub $(GRUB_BUILD_DIR)/isodir/mod
	@cp $(KERNEL) $(GRUB_BUILD_DIR)/isodir/boot/.
	@cp $(INITRD) $(GRUB_BUILD_DIR)/isodir/mod/.
	@cp $(GRUB_SRC_DIR)/grub.cfg $(GRUB_BUILD_DIR)/isodir/boot/grub/
	@echo "Copied all image components to '$(GRUB_BUILD_DIR)'."
	@echo "[`date "+%H:%M:%S"`] Creating '$@'..."
	@grub-mkrescue -o $(ISO) $(GRUB_BUILD_DIR)/isodir
	@echo "[`date "+%H:%M:%S"`] Created '$@'."

$(INITRD) : userland $(INITRD_DEPS)
	@mkdir -p $(SYSROOT_BUILD_DIR)
	@cp -r $(SYSROOT_SRC_DIR)/* $(SYSROOT_BUILD_DIR)/.
	@echo "[`date "+%H:%M:%S"`] Creating '$@'..."
	@cd $(SYSROOT_BUILD_DIR) && $(TAR) cf $(INITRD) --format=ustar *
	@echo "[`date "+%H:%M:%S"`] Created '$@'."

$(KERNEL) : kernel

kernel :
	@mkdir -p $(KERNEL_BUILD_ROOT)
	@echo "[`date "+%H:%M:%S"`] Building kernel..."
	@cd $(KERNEL_SRC_DIR) && $(MAKE) all $(KERNEL_MAKE_ARGS)
	@echo "[`date "+%H:%M:%S"`] Built kernel."

userland :
	@mkdir -p $(USERLAND_BUILD_DIR)
	@echo "[`date "+%H:%M:%S"`] Building kernel..."
	@cd $(USERLAND_SRC_DIR) && $(MAKE) all $(USERLAND_MAKE_ARGS)
	@echo "[`date "+%H:%M:%S"`] Built kernel."
