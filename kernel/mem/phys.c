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
#include <tupai/proc.h>
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
};

typedef struct entry
{
	uint8_t flags;
	proc_t* proc;
} entry_t;

uint64_t phys_preload_size = 0;

static entry_t* map;
static size_t map_size;

static entry_t make_entry(uint8_t flags, proc_t* proc);

void phys_init()
{
	map_size = align_up(phys_preload_size, PAGE_SIZE) / PAGE_SIZE;
	map = alloc(sizeof(entry_t) * map_size);

	for (size_t i = 0; i < map_size; i ++)
		map[i] = make_entry(KERNEL | MOVABLE | RAM | FREE, nullptr);

	log("[ OK ] Physical memory allocator initiated with %u entries\n", (uint)map_size);
}

entry_t make_entry(uint8_t flags, proc_t* proc)
{
	return (entry_t){ .flags = flags, .proc = proc };
}
