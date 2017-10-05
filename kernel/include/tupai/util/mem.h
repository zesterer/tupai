//
// file : mem.h
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

#ifndef TUPAI_UTIL_MEM_H
#define TUPAI_UTIL_MEM_H

#include <tupai/mem/kheap.h>
#include <tupai/util/panic.h>

#define DEFAULT_ALIGN 8

inline uintptr_t align_up(uintptr_t x, size_t a)
{
	return (x % a == 0) ? x : ((x / a + 1) * a);
}

inline uintptr_t align_down(uintptr_t x, size_t a)
{
	return (x / a) * a;
}

void* alloc(size_t n);
void* alloc_aligned(size_t n, size_t align);
void dealloc(void* ptr);

#endif
