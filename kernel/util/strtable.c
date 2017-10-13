//
// file : strtable.c
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

#include <tupai/util/strtable.h>
#include <tupai/util/mem.h>
#include <tupai/util/str.h>
#include <tupai/util/panic.h>

typedef struct strtable_entry
{
	bool used;
	char* key;
	void* ptr;
} strtable_entry_t;

static size_t hash(const char* str);
static void resize(strtable_t* table, size_t ncap);
static int insert(strtable_t* table, char* key, void* ptr);

int strtable_create(strtable_t* table)
{
	*table = (strtable_t){ .size = 0, .cap = 1, .entries = ALLOC_ARR(strtable_entry_t, 1) };

	for (size_t i = 0; i < table->cap; i ++)
		table->entries[i].used = false;

	return 0;
}

int strtable_add(strtable_t* table, const char* key, void* ptr)
{
	if (table->size >= table->cap)
		resize(table, table->cap * 2);

	int val = insert(table, str_new(key), ptr);
	if (val == 0)
		table->size ++;

	 return val;
}

void* strtable_get(strtable_t* table, const char* key)
{
	size_t off = hash(key) % table->cap;
	for (size_t i = 0; i < table->cap; i ++)
	{
		size_t j = (off + i) % table->cap;
		if (table->entries[j].used && str_equal(table->entries[j].key, key))
			return table->entries[j].ptr;
	}

	return nullptr;
}

void* strtable_nth(strtable_t* table, size_t n)
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

int strtable_key(strtable_t* table, void* val, char* buff)
{
	for (size_t i = 0; i < table->cap; i ++)
	{
		if (table->entries[i].used && table->entries[i].ptr == val)
		{
			str_copy(table->entries[i].key, buff);
			return 0;
		}
	}

	return 1;
}

void strtable_delete(strtable_t* table)
{
	dealloc(table->entries);
}

size_t hash(const char* str)
{
	uint32_t hash = 5381;
	while (*(str++) != '\0')
		hash = ((hash << 5) + hash) + *str;
	return hash;
}

void resize(strtable_t* table, size_t ncap)
{
	size_t ocap = table->cap;
	strtable_entry_t* oentries = table->entries;

	// Allocate new table
	table->cap = ncap;
	table->entries = ALLOC_ARR(strtable_entry_t, table->cap);

	// Mark all new entries as unused
	for (size_t i = 0; i < table->cap; i ++)
		table->entries[i].used = false;

	// Copy across used entries
	for (size_t i = 0; i < ocap; i ++)
	{
		if (oentries[i].used)
			insert(table, oentries[i].key, oentries[i].ptr);
	}

	// Deallocate old table
	dealloc(oentries);
}

int insert(strtable_t* table, char* key, void* ptr)
{
	size_t off = hash(key) % table->cap;
	for (size_t i = 0; i < table->cap; i ++)
	{
		size_t j = (off + i) % table->cap;
		if (!table->entries[j].used)
		{
			table->entries[j] = (strtable_entry_t){ .used = true, .key = key, .ptr = ptr };
			return 0;
		}
	}

	return 1;
}
