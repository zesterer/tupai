//
// file : mem.c
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

#include <tupai/util/mem.h>
#include <tupai/util/panic.h>
#include <tupai/mem/kheap.h>

#define DEFAULT_ALIGN 8

void* alloc(size_t n)
{
	return alloc_aligned(n, DEFAULT_ALIGN);
}

void* alloc_aligned(size_t n, size_t align)
{
	void* ret;
	switch (pool_alloc(&kheap, n, align, &ret))
	{
	case 0:
		return ret;
	case 1:
		panic("No space left to allocate in kernel heap");
	case 2:
		panic("Serious kernel heap corruption occured when attempting to allocate");
	default:
		panic("Unknown kernel heap error occured when attempting to allocate");
	}
}

void dealloc(void* ptr)
{
	switch (pool_dealloc(&kheap, ptr))
	{
	case 0:
		return;
	case 1:
		panic("Attempted to deallocate invalid pointer from kernel heap");
	case 2:
		panic("Serious kernel heap corruption occured when attempting to deallocate");
	default:
		panic("Unknown kernel heap error occured when attempting to deallocate");
	}
}
