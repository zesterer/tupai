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
LIBC_SRC_DIR    = $(SRC_DIR)/libc
LIBC_BUILD_DIR  = $(BUILD_DIR)/libc
LIBC_MAKE_ARGS  = BUILD_DIR=$(LIBC_BUILD_DIR) SYSROOT=$(SYSROOT) TARGET_FAMILY=$(TARGET_FAMILY) TARGET_ARCH=$(TARGET_ARCH)

# Rules
# -----

.PHONY : all libc

all : libc

libc :
	@mkdir -p $(LIBC_BUILD_DIR)
	@echo "[`date "+%H:%M:%S"`] Building libc..."
	@cd $(LIBC_SRC_DIR) && $(MAKE) all $(LIBC_MAKE_ARGS)
	@echo "[`date "+%H:%M:%S"`] Built libc."
