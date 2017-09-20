//
// file : mem.hpp
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

#ifndef TUPAI_UTIL_MEM_HPP
#define TUPAI_UTIL_MEM_HPP

// Tupai
#include <tupai/mem/kmem.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static constexpr size_t align_floor(size_t n, size_t align)
		{
			return n - n % align;
		}

		static constexpr size_t align_ceiling(size_t n, size_t align)
		{
			return (n == 0) ? align : (align + align * ((n - 1) / align));
		}

		static __attribute__ ((unused)) void mem_copy(const void* src, void* dest, size_t n)
		{
			for (size_t i = 0; i < n; i ++)
				((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
		}

		template <typename T, size_t N>
		static constexpr size_t length(T (&arr)[N]) { (void)arr; return N; }

		static void* mem_alloc(size_t n, size_t align = sizeof(size_t))
		{
			return mem::kmem::alloc(n, align);
		}

		static void mem_dealloc(void* ptr)
		{
			return mem::kmem::dealloc(ptr);
		}
	}
}

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { }
inline void  operator delete[](void *, void *) throw() { }

#endif
