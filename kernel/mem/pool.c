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
#include <tupai/util/log.h>
#include <tupai/def.h>

#define BPBL2 2 // Blocks per byte, log2 (i.e: 4 blocks per byte)
#define BPB (1 << BPBL2)

enum
{
	ERROR = 0b00,
	HEAD = 0b01,
	TAIL = 0b10,
	FREE = 0b11,
};

static int block_get(pool_t* pool, size_t index);
static void block_set(pool_t* pool, size_t index, int value);

/*
** pool_init()
** Initiate a pool descriptor type (assign a suitable map, block size, etc.)
*/


int pool_init(pool_t* pool, size_t start, size_t size, size_t block_size)
{
	pool->start = align_up(start, PAGE_SIZE);
	pool->size = size - (pool->start - start);

	pool->block_size = block_size;
	pool->block_count = (pool->size * BPB) / (pool->block_size * BPB + 1);

	if (pool->block_count <= 0) // Is it large enough to even represent a valid pool?
		return 1;
	else
	{
		pool->map = (uint8_t*)((pool->start + pool->size) - align_up(pool->block_count, BPB) / BPB);

		for (size_t i = 0; i < pool->block_count; i ++)
			block_set(pool, i, FREE);

		return 0;
	}
}

/*
** pool_alloc()
** Allocate a region of heap memory with at least the given size
*/


int pool_alloc(pool_t* pool, size_t n, size_t align, void** ret)
{
	size_t blocks = align_up(n, pool->block_size) / pool->block_size;

	for (size_t i = 0; i < pool->block_count; i ++)
	{
		int type = block_get(pool, i);
		if (type == ERROR)
			return 2;
		else if (type == FREE && (pool->start + i * pool->block_size) % align == 0)
		{
			for (size_t j = i; j < pool->block_count; j ++)
			{
				int type = block_get(pool, i);
				if (type == ERROR)
					return 2;
				else if (j - i == blocks)
				{
					*ret = (void*)(pool->start + i * pool->block_size);
					block_set(pool, i, HEAD);
					for (i ++; i < j; i ++)
						block_set(pool, i, TAIL);
					return 0;
				}
				else if (type != FREE)
				{
					i = j;
					break;
				}
			}
		}
	}

	return 1;
}

/*
** pool_dealloc()
** Deallocate a heap region that starts at the given pointer
*/

int pool_dealloc(pool_t* pool, void* ptr)
{
	if (((size_t)ptr - pool->start) % pool->block_size != 0)
		return 1;
	if (((size_t)ptr < pool->start || (size_t)ptr > pool->start + pool->block_count * pool->block_size))
		return 1;

	size_t i = ((size_t)ptr - pool->start) / pool->block_size;
	int type = block_get(pool, i);
	if (type == ERROR)
		return 2;
	else if (type == HEAD)
	{
		block_set(pool, i, FREE);
		for (i ++; i < pool->block_count; i ++)
		{
			int type = block_get(pool, i);
			if (type == ERROR)
				return 2;
			else if (type != TAIL)
				break;
			else
				block_set(pool, i, FREE);
		}
		return 0;
	}
	else
		return 1;

}

/*
** pool_integrity_check()
** Performs basic pool map checks (i.e: no error entries, TAIL never follows FREE, etc.)
*/

int pool_integrity_check(pool_t* pool)
{
	bool cfree = true;
	for (size_t i = 0; i < pool->block_count; i ++)
	{
		int type = block_get(pool, i);
		if (type == ERROR)
			return 1;
		else if (type == FREE)
			cfree = true;
		else if (type == TAIL && cfree)
			return 1;
		else
			cfree = false;
	}
	return 0;
}

void pool_display(pool_t* pool, size_t n)
{
	for (size_t i = 0; i < pool->block_count && i < n; i ++)
	{
		switch (block_get(pool, i))
		{
			case HEAD:
				logc('H');
				break;
			case TAIL:
				logc('T');
				break;
			case FREE:
				logc('-');
				break;
			default:
				logc('!');
				break;
		}
	}

	logc('\n');
}

static int block_get(pool_t* pool, size_t index)
{
	int off = (index % BPB) * (8 / BPB);
	return (int)((pool->map[index >> BPBL2] & (0b11 << off)) >> off);
}

static void block_set(pool_t* pool, size_t index, int value)
{
	int off = (index % BPB) * (8 / BPB);
	pool->map[index >> BPBL2] &= ~(0b11 << off);
	pool->map[index >> BPBL2] |= (value & 0b11) << off;
}
