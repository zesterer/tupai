//
// file : phys.h
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

#ifndef TUPAI_MEM_PHYS_H
#define TUPAI_MEM_PHYS_H

#include <tupai/proc.h>
#include <tupai/type.h>

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

extern uint64_t phys_preload_size;

void phys_init();
void phys_set(uintptr_t offset, uint8_t flags, proc_t* proc);
void phys_set_region(uintptr_t offset, size_t size, uint8_t flags, proc_t* proc);
void phys_display();

#endif
