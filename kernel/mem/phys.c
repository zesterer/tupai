//
// file : phys.c
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

#include <tupai/mem/phys.h>
#include <tupai/util/log.h>
#include <tupai/util/mem.h>
#include <tupai/def.h>

enum
{
	KERNEL  = (1 << 0),
	USER    = 0,

	STATIC  = (1 << 1),
	MOVABLE = 0,

	SPECIAL = (1 << 2),
	RAM     = 0,

	USED    = (1 << 3),
	FREE    = 0,

	SHARED  = (1 << 4),
};

typedef struct entry
{
	uint8_t flags;
	proc_t* proc;
} entry_t;

uint64_t phys_preload_size = 0;

extern char kernel_end[];

static entry_t* map;
static size_t map_size;

static entry_t make_entry(uint8_t flags, proc_t* proc);
static size_t entry_to_ptr(size_t i);

void phys_init()
{
	map_size = align_up(phys_preload_size, PAGE_SIZE) / PAGE_SIZE;
	map = alloc(sizeof(entry_t) * map_size);

	// Set blank memory initially
	for (size_t i = 0; i < map_size; i ++)
		map[i] = make_entry(KERNEL | MOVABLE | RAM | FREE, nullptr);

	logf("[ OK ] Physical memory allocator initiated with %u entries\n", (uint)map_size);

	// Reclaim the kernel
	phys_set_region(0, (size_t)kernel_end, KERNEL | MOVABLE | RAM | USED, nullptr);
	logf("[ OK ] Reclaimed kernel memory between %p and %p\n", 0, (void*)kernel_end);
}

void phys_set(size_t offset, uint8_t flags, proc_t* proc)
{
	size_t index = align_down(offset, PAGE_SIZE) / PAGE_SIZE;
	map[index].flags = flags;
	map[index].proc = proc;
}

void phys_set_region(size_t offset, size_t size, uint8_t flags, proc_t* proc)
{
	size_t index = align_down(offset, PAGE_SIZE) / PAGE_SIZE;
	size_t entries = align_up(offset + size, PAGE_SIZE) / PAGE_SIZE - index;

	for (size_t i = 0; i < entries; i ++)
	{
		map[i].flags = flags;
		map[i].proc = proc;
	}
}

void phys_display()
{
	uint8_t cflags = map[0].flags;
	proc_t* cproc = map[0].proc;
	size_t i = 0;

	for (; i < map_size; i ++)
	{
		if (i == 0 || map[i].flags != cflags || map[i].proc != cproc)
		{
			cflags = map[i].flags;
			cproc = map[i].proc;
			logf("%p : flags = %b, proc = %p\n", (void*)entry_to_ptr(i), cflags, (void*)cproc);
		}
	}
	logf("%p : MEMORY_LIMIT\n", (void*)entry_to_ptr(i));
}

entry_t make_entry(uint8_t flags, proc_t* proc)
{
	return (entry_t){ .flags = flags, .proc = proc };
}

size_t entry_to_ptr(size_t i)
{
	return i * PAGE_SIZE;
}
