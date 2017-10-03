//
// file : virt.c
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

#include <tupai/mem/virt.h>
#include <tupai/util/mem.h>
#include <tupai/def.h>

#define TABLE1_LEN 1024 // Page directory (4M each)
#define TABLE0_LEN 1024 // Page table (4K each)

#define GLOBAL (1 << 7)
#define DIRTY (1 << 6)
#define ACCESSED (1 << 5)
#define NOCACHE (1 << 4)
#define WRITETHROUGH (1 << 3)
#define USER (1 << 2)
#define READWRITE (1 << 1)
#define PRESENT (1 << 0)

static uint32_t* entry_to_ptr(uint32_t entry);

int virt_init(virt_t* virt, uint8_t flags)
{
	virt->flags = flags;

	*((uint32_t**)&virt->table) = alloc(sizeof(uint32_t) * TABLE1_LEN);

	for (size_t i = 0; i < TABLE1_LEN; i ++) // Clear out the table
		((uint32_t*)virt->table)[i] = 0;

	return 0;
}

int virt_map(virt_t* virt, size_t virt_page, size_t phys_page)
{
	uint32_t* table = (uint32_t*)virt->table;

	if ((table[virt_page / TABLE0_LEN] & PRESENT) == 0)
	{
		uint32_t* new_table = alloc(sizeof(uint32_t) * TABLE0_LEN);

		for (size_t i = 0; i < TABLE0_LEN; i ++) // Clear out the table
			new_table[i] = 0;

		table[virt_page / TABLE0_LEN] = (uint32_t)new_table | ((virt->flags & VIRT_KERNEL) ? 0 : USER) | READWRITE | PRESENT; // Set the new page directory entry and add flags to it
	}

	entry_to_ptr(table[virt_page / TABLE0_LEN])[virt_page % TABLE0_LEN] = ((uint32_t)phys_page * PAGE_SIZE)| ((virt->flags & VIRT_KERNEL) ? 0 : USER) | READWRITE | PRESENT; // Set the new page table entry and add flags to it

	return 0;
}

int virt_delete(virt_t* virt);

void virt_enable(virt_t* virt)
{
	size_t ptr = (size_t)virt->table - VIRT_OFFSET;
	asm volatile ("mov %0, %%eax; mov %%eax, %%cr3;" : "=r"(ptr) :: "%eax");
}

static uint32_t* entry_to_ptr(uint32_t entry)
{
	return (uint32_t*)(entry & 0xFFFFFF00);
}
