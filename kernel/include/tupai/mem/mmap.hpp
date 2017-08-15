//
// file : mmap.hpp
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

#ifndef TUPAI_MEM_MMAP_HPP
#define TUPAI_MEM_MMAP_HPP

// Tupai
#include <tupai/task/task.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace mem
	{
		namespace mmap
		{
			enum class page_flags : uint8_t
			{
				STATIC = 1 << 0,

				NONE = 0,
			};

			struct page_t
			{
				task::proc_ptr_t _owner;
				uint8_t          _flags;
			};

			void init();
			int  alloc(void** phys_addr, task::proc_ptr_t owner, uint8_t flags);
			int  alloc_contiguous(void** phys_addr, size_t size, task::proc_ptr_t owner, uint8_t flags);
			int  reserve(void* phys_addr, task::proc_ptr_t owner, uint8_t flags);
			int  reserve_region(void* phys_addr, size_t size, task::proc_ptr_t owner, uint8_t flags);
			int  dealloc(void* phys_addr);
			void display();
		}
	}
}

#endif
