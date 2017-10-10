//
// file : tar.c
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

#include <tupai/util/tar.h>
#include <tupai/util/mem.h>

tar_entry_t* tar_next(const tar_entry_t* entry)
{
	uintptr_t offset = align_up(512 + tar_size(entry), 512);
	tar_entry_t* nentry = (tar_entry_t*)((uintptr_t)entry + offset);

	if (nentry->filename[0] == '\0')
		return nullptr;
	else
		return nentry;
}

size_t tar_size(const tar_entry_t* entry)
{
	size_t sz = 0;
	for (int i = 0; i < 12 && entry->size[i] != '\0' && i < 12; i ++)
	{
		sz *= 8;
		sz += (size_t)(entry->size[i] - '0');
	}

	return sz;
}

uint8_t* tar_data(const tar_entry_t* entry)
{
	return (uint8_t*)((uintptr_t)entry + 512);
}
