//
// file : pool.c
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

#include <tupai/mem/pool.h>
#include <tupai/util/mem.h>
#include <tupai/def.h>

int pool_init(pool_t* pool, size_t start, size_t size, size_t block_size)
{
	pool->start = align_up(start, PAGE_SIZE);
	pool->size = size - (pool->start - start);

	pool->block_size = block_size;
	pool->block_count = (pool->size * 4) / (pool->block_size * 4 + 1);

	if (pool->block_count <= 0) // Is it large enough to even represent a valid pool?
		return 1;
	else
	{
		pool->map = (uint8_t*)((pool->start + pool->size) - align_up(pool->block_count, 4) / 4);
		return 0;
	}
}
