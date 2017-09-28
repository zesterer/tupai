//
// file : pool.h
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

#ifndef TUPAI_MEM_POOL_H
#define TUPAI_MEM_POOL_H

#include <tupai/type.h>

typedef struct pool
{
	size_t start;
	size_t size;

	uint8_t* map;
	size_t block_count;
	size_t block_size;
} pool_t;

int pool_init(pool_t* pool, size_t start, size_t size, size_t block_size);
int pool_alloc(pool_t* pool, size_t n, size_t align, void** ret);
int pool_dealloc(pool_t* pool, void* ptr);
int pool_integrity_check(pool_t* pool);

#endif
