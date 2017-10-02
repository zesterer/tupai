//
// file : conv.h
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

#ifndef TUPAI_UTIL_CONV_H
#define TUPAI_UTIL_CONV_H

#include <tupai/type.h>

#define I8_STR_MAX (sizeof(int8_t) * 8 + 2)
#define I16_STR_MAX (sizeof(int16_t) * 8 + 2)
#define I32_STR_MAX (sizeof(int32_t) * 8 + 2)
#define I64_STR_MAX (sizeof(int64_t) * 8 + 2)

#define U8_STR_MAX (sizeof(uint8_t) * 8 + 1)
#define U16_STR_MAX (sizeof(uint16_t) * 8 + 1)
#define U32_STR_MAX (sizeof(uint32_t) * 8 + 1)
#define U64_STR_MAX (sizeof(uint64_t) * 8 + 1)

#define PTR_STR_MAX (sizeof(size_t) * 8 + 1)

bool i8_to_str(int8_t x, int base, int pad, char (str)[I8_STR_MAX]);
bool i16_to_str(int16_t x, int base, int pad, char (str)[I16_STR_MAX]);
bool i32_to_str(int32_t x, int base, int pad, char (str)[I32_STR_MAX]);
bool i64_to_str(int64_t x, int base, int pad, char (str)[I64_STR_MAX]);

bool u8_to_str(uint8_t x, int base, int pad, char (str)[U8_STR_MAX]);
bool u16_to_str(uint16_t x, int base, int pad, char (str)[U16_STR_MAX]);
bool u32_to_str(uint32_t x, int base, int pad, char (str)[U32_STR_MAX]);
bool u64_to_str(uint64_t x, int base, int pad, char (str)[U64_STR_MAX]);

bool ptr_to_str(size_t x, int base, int pad, char (str)[PTR_STR_MAX]);

#endif
