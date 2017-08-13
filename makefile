# Definitions
# -----------

TARGET_FAMILY = X86
TARGET_ARCH   = AMD64

SRC_ROOT    = $(abspath .)
BUILD_ROOT ?= $(SRC_ROOT)/build

KERNEL_SRC_DIR    = $(SRC_ROOT)/kernel
KERNEL_BUILD_ROOT = $(BUILD_ROOT)/kernel
KERNEL            = $(KERNEL_BUILD_ROOT)/tupai.elf
KERNEL_MAKE_ARGS  = BUILD_ROOT=$(KERNEL_BUILD_ROOT) LIBC_INC=$(SRC_ROOT)/include KERNEL=$(KERNEL) TARGET_FAMILY=$(TARGET_FAMILY) TARGET_ARCH=$(TARGET_ARCH)

SYSROOT_SRC_DIR = $(SRC_ROOT)/sysroot
INITRD          = $(BUILD_ROOT)/initrd.tar
INITRD_DEPS     = $(shell find $(SYSROOT_SRC_DIR) -name '*')

GRUB_BUILD_DIR = $(BUILD_ROOT)/grub
GRUB_SRC_DIR   = grub/

ISO = $(BUILD_ROOT)/tupai.iso

# Tools
# -----

TAR = tar

ifeq ($(TARGET_ARCH), AMD64)
	QEMU = qemu-system-x86_64
endif
ifeq ($(TARGET_ARCH), I386)
	QEMU = qemu-system-i686
endif
ifeq ($(TARGET_ARCH), RPI2)
	QEMU = qemu-system-arm
endif

# Rules
# -----

.PHONY : all build rebuild clean run

all : build

build : $(ISO)

rebuild : clean $(ISO)

clean :
	@rm -r -f $(ISO) $(GRUB_BUILD_DIR) $(INITRD)
	@cd $(KERNEL_SRC_DIR) && $(MAKE) clean $(KERNEL_MAKE_ARGS)
	@echo "Cleaned all."

run : $(ISO)
	@echo "Running '$(ISO)'..."
	@$(QEMU) -cdrom $(ISO)

$(ISO) : $(KERNEL) $(INITRD)
	@mkdir -p $(GRUB_BUILD_DIR)/isodir/boot/grub $(GRUB_BUILD_DIR)/isodir/mod
	@cp $(KERNEL) $(GRUB_BUILD_DIR)/isodir/boot/.
	@cp $(INITRD) $(GRUB_BUILD_DIR)/isodir/mod/.
	@cp $(GRUB_SRC_DIR)/grub.cfg $(GRUB_BUILD_DIR)/isodir/boot/grub/
	@echo "Copied all ISO components to '$(GRUB_BUILD_DIR)'."
	@grub-mkrescue -o $(ISO) $(GRUB_BUILD_DIR)/isodir
	@echo "Created '$@'."

$(INITRD) : $(INITRD_DEPS)
	@cd $(SYSROOT_SRC_DIR) && $(TAR) cf $(INITRD) --format=ustar *
	@echo "[`date "+%H:%M:%S"`] Created '$@'."

$(KERNEL) :
	@cd $(KERNEL_SRC_DIR) && $(MAKE) all $(KERNEL_MAKE_ARGS)
	@echo "[`date "+%H:%M:%S"`] Built '$@'."
