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

#ifdef ARCH_TARGET_i386
	typedef uint32_t entry_t;

	enum
	{
		ENTRY_EMPTY = 0,
		ENTRY_PTR = 0xFFFFFF00,

		ENTRY_PRESENT = (1 << 0),
	};

	#define TABLE_LAYERS 2

	static const size_t TABLE_LEN[TABLE_LAYERS] = { 1024, 1024 };
	static const size_t TABLE_TOTAL[TABLE_LAYERS] = { 1024, 1024 * 1024 };
#endif

static entry_t* entry_to_ptr(entry_t entry);
static void clear_table(entry_t* table, size_t layer);
static entry_t* alloc_table(size_t layer);
static void delete_table(entry_t* table, size_t layer);

int virt_init(virt_t* virt)
{
	virt->table = (entry_t*)(VIRT_OFFSET + (size_t)alloc_table(TABLE_LAYERS - 1));

	return 0;
}

int virt_delete(virt_t* virt)
{
	delete_table(virt->table, TABLE_LAYERS - 1);

	return 0;
}

int virt_switch(virt_t* virt)
{
	size_t ptr = (size_t)virt->table - VIRT_OFFSET;
	asm volatile ("mov %0, %%eax; mov %%eax, %%cr3" :: "r"(ptr) : "eax");
	return 0;
}

int virt_map(virt_t* virt, uintptr_t vaddr, uintptr_t paddr, uint8_t flags)
{
	(void)flags; // Unused

	entry_t* table = virt->table;

	size_t vindex = vaddr / PAGE_SIZE;

	size_t layer = TABLE_LAYERS - 1;
	entry_t* ctable = table;
	entry_t* centry = nullptr;

	while (true)
	{
		if (layer > 0)
		{
			size_t nindex = vindex / TABLE_TOTAL[layer - 1];

			if ((ctable[nindex] & ENTRY_PRESENT) == 0)
			{
				// Alloc new table
				ctable[nindex] = (entry_t)alloc_table(layer - 1);
				ctable[nindex] |= 6 | ENTRY_PRESENT; // Hack! TODO : Fix this properly
			}

			ctable = entry_to_ptr(ctable[nindex]);

			vindex -= nindex * TABLE_TOTAL[layer - 1];
			layer --;
		}
		else
		{
			centry = &ctable[vindex];
			break;
		}
	}

	*centry = ((entry_t)paddr) | 6 | ENTRY_PRESENT; // Hack! TODO : Fix this properly

	return 0;
}

int virt_map_region(virt_t* virt, uintptr_t vaddr, uintptr_t paddr, size_t size, uint8_t flags)
{
	size_t pages = align_up(size, PAGE_SIZE) / PAGE_SIZE;

	for (size_t i = 0; i < pages; i ++)
		virt_map(virt, vaddr + PAGE_SIZE * i, paddr + PAGE_SIZE * i, flags);

	return 0;
}

entry_t* entry_to_ptr(entry_t entry)
{
	return (entry_t*)((entry + VIRT_OFFSET) & ENTRY_PTR);
}

void clear_table(entry_t* table, size_t layer)
{
	for (size_t i = 0; i < TABLE_LEN[layer]; i ++)
		table[i] = ENTRY_EMPTY;
}

entry_t* alloc_table(size_t layer)
{
	entry_t* table = alloc_aligned(sizeof(entry_t) * TABLE_LEN[layer], PAGE_SIZE);
	clear_table(table, layer);
	return (entry_t*)((size_t)table - VIRT_OFFSET);
}

void delete_table(entry_t* table, size_t layer)
{
	if (layer > 0)
	{
		for (size_t i = 0; i < TABLE_LEN[layer]; i ++)
		{
			if (table[i] & ENTRY_PRESENT)
				delete_table(entry_to_ptr(table[i]), layer - 1);
		}
	}

	dealloc(table);
}
