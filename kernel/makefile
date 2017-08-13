# Definitions
# -----------

TARGET_FAMILY ?= X86
TARGET_ARCH   ?= AMD64

SRC_ROOT    = $(abspath .)
BUILD_ROOT ?= $(SRC_ROOT)/build

KERNEL ?= $(BUILD_ROOT)/tupai.elf

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

AS  = x86_64-elf-g++
CC  = x86_64-elf-g++
CPP = x86_64-elf-g++
LD  = x86_64-elf-g++

# Source files
# ------------

SRC_DIR = $(abspath .)

SRCS_REL += $(wildcard $(SRC_DIR)/*.s      $(SRC_DIR)/*.c      $(SRC_DIR)/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/dev/*.s  $(SRC_DIR)/dev/*.c  $(SRC_DIR)/dev/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/libk/*.s $(SRC_DIR)/libk/*.c $(SRC_DIR)/libk/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/mem/*.s  $(SRC_DIR)/mem/*.c  $(SRC_DIR)/mem/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/sys/*.s  $(SRC_DIR)/sys/*.c  $(SRC_DIR)/sys/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/task/*.s $(SRC_DIR)/task/*.c $(SRC_DIR)/task/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/util/*.s $(SRC_DIR)/util/*.c $(SRC_DIR)/util/*.cpp)
SRCS_REL += $(wildcard $(SRC_DIR)/vfs/*.s  $(SRC_DIR)/vfs/*.c  $(SRC_DIR)/vfs/*.cpp)

ifeq ($(TARGET_FAMILY), X86)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/*.s $(SRC_DIR)/x86/*.c $(SRC_DIR)/x86/*.cpp)
endif
ifeq ($(TARGET_FAMILY), ARM)
	SRCS_REL += $(wildcard $(SRC_DIR)/arm/*.s $(SRC_DIR)/arm/*.c $(SRC_DIR)/arm/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/arm/util/*.s $(SRC_DIR)/arm/util/*.c $(SRC_DIR)/arm/util/*.cpp)
endif

ifeq ($(TARGET_ARCH), AMD64)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/amd64/*.s $(SRC_DIR)/x86/amd64/*.c $(SRC_DIR)/x86/amd64/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/amd64/crt/*.s $(SRC_DIR)/x86/amd64/crt/*.c $(SRC_DIR)/x86/amd64/crt/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/amd64/sys/*.s $(SRC_DIR)/x86/amd64/sys/*.c $(SRC_DIR)/x86/amd64/sys/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/amd64/util/*.s $(SRC_DIR)/x86/amd64/util/*.c $(SRC_DIR)/x86/amd64/util/*.cpp)
endif
ifeq ($(TARGET_ARCH), I386)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/i386/*.s $(SRC_DIR)/x86/i386/*.c $(SRC_DIR)/x86/i386/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/i386/crt/*.s $(SRC_DIR)/x86/i386/crt/*.c $(SRC_DIR)/x86/i386/crt/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/i386/sys/*.s $(SRC_DIR)/x86/i386/sys/*.c $(SRC_DIR)/x86/i386/sys/*.cpp)
	SRCS_REL += $(wildcard $(SRC_DIR)/x86/i386/util/*.s $(SRC_DIR)/x86/i386/util/*.c $(SRC_DIR)/x86/i386/util/*.cpp)
endif
ifeq ($(TARGET_ARCH), RPI2)
	SRCS_REL += $(wildcard $(SRC_DIR)/rpi2/*.s $(SRC_DIR)/rpi2/*.c $(SRC_DIR)/rpi2/*.cpp)
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

all : $(KERNEL)

clean :
	@rm -r -f $(KERNEL) $(OBJS) $(DEPS) $(BDIRS)

$(KERNEL) : $(OBJS)
	@$(LD) $(CFLAGS) -T $(LSCRIPT) -o $@ $(CRTBEGIN) $(OBJS) $(CRTEND) $(LFLAGS)
	@echo "[`date "+%H:%M:%S"`] Linked '$@'."

$(OBJS) : | $(BDIRS)

$(BDIRS) :
	@mkdir -p $(BDIRS)
	@echo "Created build directory '$@'."

-include $(DEPS)

# Compile .s files
$(BUILD_ROOT)/%.o : $(SRC_DIR)/%.s
	@$(AS) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$<'."

# Compile .c files
$(BUILD_ROOT)/%.o : $(SRC_DIR)/%.c
	@$(CC) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$<'."

# Compile .cpp files
$(BUILD_ROOT)/%.o : $(SRC_DIR)/%.cpp
	@$(CPP) -MMD -c -o $@ $< $(CFLAGS)
	@echo "[`date "+%H:%M:%S"`] Compiled '$<'."
