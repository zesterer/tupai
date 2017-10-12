//
// file : strtable.h
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

#ifndef TUPAI_UTIL_STRTABLE_H
#define TUPAI_UTIL_STRTABLE_H

#include <tupai/type.h>

typedef struct strtable_entry strtable_entry_t;

typedef struct strtable
{
	size_t size;
	size_t cap;
	strtable_entry_t* entries;
} strtable_t;

int strtable_create(strtable_t* table);
int strtable_add(strtable_t* table, const char* key, void* ptr);
void* strtable_get(strtable_t* table, const char* key);
void* strtable_nth(strtable_t* table, size_t n);
void strtable_delete(strtable_t* table);

#endif
