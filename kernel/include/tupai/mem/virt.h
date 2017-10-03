//
// file : virt.h
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

#ifndef TUPAI_MEM_VIRT_H
#define TUPAI_MEM_VIRT_H

#include <tupai/type.h>

#define VIRT_KERNEL (1 << 0)

typedef struct virt
{
	void* table; // Internal table. Platform-dependent
	uint8_t flags;
} virt_t;

int virt_init(virt_t* virt, uint8_t flags);
int virt_map(virt_t* virt, size_t virt_page, size_t phys_page); // Uses page indexes, not real addresses!
int virt_delete(virt_t* virt);
void virt_enable(virt_t* virt);

#endif
