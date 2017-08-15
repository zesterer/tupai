//
// file : cpp.cpp
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

// Tupai
#include <tupai/mem/kmem.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

extern "C" void* memset(void* ptr, int value, size_t num)
{
	for (size_t i = 0; i < num; i ++)
		((uint8_t*)ptr)[i] = (uint8_t)value;
	return ptr;
}

extern "C" void* memcpy(void* dest, const void* src, size_t n)
{
	for (size_t i = 0; i < n; i ++)
		((uint8_t*)dest)[i] = ((const uint8_t*)src)[i];
	return dest;
}

void* operator new(size_t n)
{
	return tupai::mem::kmem::alloc(n);
}

void* operator new[](size_t n)
{
	return tupai::mem::kmem::alloc(n);
}

void operator delete(void* ptr)
{
	tupai::mem::kmem::dealloc(ptr);
}

void operator delete[](void* ptr)
{
	tupai::mem::kmem::dealloc(ptr);
}

void operator delete(void* ptr, size_t size __attribute__ ((unused)))
{
	tupai::mem::kmem::dealloc(ptr);
}

void operator delete[](void* ptr, size_t size __attribute__ ((unused)))
{
	tupai::mem::kmem::dealloc(ptr);
}

//void* __dso_handle;

#if defined(ARCH_FAMILY_X86)
	extern "C" int  __cxa_atexit(void (*destructor) (void*) __attribute__ ((unused)), void* arg __attribute__ ((unused)), void* dso __attribute__ ((unused))) { return 0; }
	extern "C" void __cxa_finalize(void* f __attribute__ ((unused))) { }
#endif

#if defined(ARCH_FAMILY_arm)
	extern "C" int  __aeabi_atexit(void* obj __attribute__ ((unused)), void (*destructor) (void*) __attribute__ ((unused)), void* dso __attribute__ ((unused))) { return 0; }
	extern "C" void __aeabi_finalize(void* f __attribute__ ((unused))) { }
#endif
