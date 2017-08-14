# Definitions
# -----------

# Architecture configuration

# X86 / ARM
TARGET_FAMILY = X86
# I386 / AMD64
TARGET_ARCH   = AMD64

SRC_ROOT    = $(abspath .)
BUILD_ROOT ?= $(SRC_ROOT)/build

# Kernel
KERNEL_SRC_DIR    = $(SRC_ROOT)/kernel
KERNEL_BUILD_ROOT = $(BUILD_ROOT)/kernel
KERNEL            = $(KERNEL_BUILD_ROOT)/tupai.elf
KERNEL_MAKE_ARGS  = BUILD_ROOT=$(KERNEL_BUILD_ROOT) LIBC_INC=$(SRC_ROOT)/include KERNEL=$(KERNEL) TARGET_FAMILY=$(TARGET_FAMILY) TARGET_ARCH=$(TARGET_ARCH)

# Initrd
SYSROOT_SRC_DIR = $(SRC_ROOT)/sysroot
INITRD          = $(BUILD_ROOT)/initrd.tar
INITRD_DEPS     = $(shell find $(SYSROOT_SRC_DIR) -name '*')

# GRUB
GRUB_BUILD_DIR = $(BUILD_ROOT)/grub
GRUB_SRC_DIR   = grub/

ISO = $(BUILD_ROOT)/tupai.iso

# Tools
# -----

TAR = tar

QEMU_ARGS = -d guest_errors -d cpu_reset --no-reboot --no-shutdown -m 256M
ifeq ($(TARGET_ARCH), AMD64)
	QEMU = qemu-system-x86_64
	QEMU_ARGS +=
endif
ifeq ($(TARGET_ARCH), I386)
	QEMU = qemu-system-i386
	QEMU_ARGS +=
endif
ifeq ($(TARGET_ARCH), RPI2)
	QEMU = qemu-system-arm
	QEMU_ARGS += -M raspi2
endif

BOCHS = bochs

# Rules
# -----

.PHONY : all build rebuild kernel clean run debug

all : build
build : $(ISO) kernel
rebuild : clean $(ISO) kernel

clean :
	@cd $(KERNEL_SRC_DIR) && $(MAKE) clean $(KERNEL_MAKE_ARGS)
	@rm -r -f $(ISO) $(GRUB_BUILD_DIR) $(INITRD) $(BUILD_ROOT)
	@echo "Cleaned all."

run : $(ISO) kernel
	@echo "Running '$(ISO)'..."
	@$(QEMU) $(QEMU_ARGS) -cdrom $(ISO)

debug : $(ISO) kernel
	@echo "Debugging '$(ISO)'..."
	@$(BOCHS)

$(ISO) : $(KERNEL) $(INITRD)
	@mkdir -p $(GRUB_BUILD_DIR)/isodir/boot/grub $(GRUB_BUILD_DIR)/isodir/mod
	@cp $(KERNEL) $(GRUB_BUILD_DIR)/isodir/boot/.
	@cp $(INITRD) $(GRUB_BUILD_DIR)/isodir/mod/.
	@cp $(GRUB_SRC_DIR)/grub.cfg $(GRUB_BUILD_DIR)/isodir/boot/grub/
	@echo "Copied all image components to '$(GRUB_BUILD_DIR)'."
	@echo "[`date "+%H:%M:%S"`] Creating '$<'..."
	@grub-mkrescue -o $(ISO) $(GRUB_BUILD_DIR)/isodir
	@echo "[`date "+%H:%M:%S"`] Created '$@'."

$(INITRD) : $(INITRD_DEPS)
	@echo "[`date "+%H:%M:%S"`] Creating '$@'..."
	@cd $(SYSROOT_SRC_DIR) && $(TAR) cf $(INITRD) --format=ustar *
	@echo "[`date "+%H:%M:%S"`] Created '$@'."

$(KERNEL) : kernel
kernel :
	@echo "[`date "+%H:%M:%S"`] Building kernel..."
	@cd $(KERNEL_SRC_DIR) && $(MAKE) all $(KERNEL_MAKE_ARGS)
	@echo "[`date "+%H:%M:%S"`] Built kernel."
