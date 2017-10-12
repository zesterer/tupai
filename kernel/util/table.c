//
// file : table.c
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

#include <tupai/util/table.h>
#include <tupai/util/mem.h>
#include <tupai/util/panic.h>

typedef struct table_entry
{
	bool used;
	id_t id;
	void* ptr;
} table_entry_t;

static size_t hash(id_t x);
static void resize(table_t* table, size_t ncap);
static int insert(table_t* table, id_t id, void* ptr);

int table_create(table_t* table)
{
	*table = (table_t){ .size = 0, .cap = 1, .entries = ALLOC_ARR(table_entry_t, 1) };

	for (size_t i = 0; i < table->cap; i ++)
		table->entries[i].used = false;

	return 0;
}

int table_add(table_t* table, id_t id, void* ptr)
{
	if (table->size >= table->cap)
		resize(table, table->cap * 2);

	int val = insert(table, id, ptr);
	if (val == 0)
		table->size ++;

	 return val;
}

void* table_get(table_t* table, id_t id)
{
	size_t off = hash(id) % table->cap;
	for (size_t i = 0; i < table->cap; i ++)
	{
		size_t j = (off + i) % table->cap;
		if (table->entries[j].used && table->entries[j].id == id)
			return table->entries[j].ptr;
	}

	return nullptr;
}

void* table_nth(table_t* table, size_t n)
{
	size_t c = 0;
	for (size_t i = 0; i < table->cap; i ++)
	{
		if (table->entries[i].used)
		{
			if (c == n)
				return table->entries[i].ptr;
			else
				c ++;
		}
	}

	return nullptr;
}

void table_delete(table_t* table)
{
	dealloc(table->entries);
}

size_t hash(id_t x)
{
	return x;
}

void resize(table_t* table, size_t ncap)
{
	size_t ocap = table->cap;
	table_entry_t* oentries = table->entries;

	// Allocate new table
	table->cap = ncap;
	table->entries = ALLOC_ARR(table_entry_t, table->cap);

	// Mark all new entries as unused
	for (size_t i = 0; i < table->cap; i ++)
		table->entries[i].used = false;

	// Copy across used entries
	for (size_t i = 0; i < ocap; i ++)
	{
		if (oentries[i].used)
			insert(table, oentries[i].id, oentries[i].ptr);
	}

	// Deallocate old table
	dealloc(oentries);
}

int insert(table_t* table, id_t id, void* ptr)
{
	size_t off = hash(id) % table->cap;
	for (size_t i = 0; i < table->cap; i ++)
	{
		size_t j = (off + i) % table->cap;
		if (!table->entries[j].used)
		{
			table->entries[j] = (table_entry_t){ .used = true, .id = id, .ptr = ptr };
			return 0;
		}
	}

	return 1;
}
