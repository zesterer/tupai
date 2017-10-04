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

SRC_DIR = $(abspath .)

# Libc
INIT_SRC_DIR    = $(SRC_DIR)/init
INIT_BUILD_DIR  = $(BUILD_DIR)/init
INIT_MAKE_ARGS  = BUILD_ROOT=$(INIT_BUILD_DIR) SYSROOT=$(SYSROOT) USERLAND_SRC=$(USERLAND_SRC) USERLAND_BUILD_DIR=$(USERLAND_BUILD_DIR) TARGET_FAMILY=$(TARGET_FAMILY) TARGET_ARCH=$(TARGET_ARCH)

# Rules
# -----

.PHONY : all init

all : init

init :
	@mkdir -p $(INIT_BUILD_DIR)
	@echo "[`date "+%H:%M:%S"`] Building init..."
	@cd $(INIT_SRC_DIR) && $(MAKE) all $(INIT_MAKE_ARGS)
	@echo "[`date "+%H:%M:%S"`] Built init."
