//
// file : virt.hpp
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

#ifndef TUPAI_MEM_VIRTUAL_HPP
#define TUPAI_MEM_VIRTUAL_HPP

// Tupai
#include <tupai/task/task.hpp>
#include <tupai/util/vector.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace mem
	{
		namespace virt
		{
			struct page_t
			{
				size_t phys_index;
				size_t virt_index;
			};

			struct space_t
			{
				task::proc_ptr_t proc;
				util::vector_t<page_t> _pages;

				void* table = nullptr;

				space_t();
				int map_page(void* phys_addr, void* virt_addr);
				int map_region(void* phys_addr, size_t size, void* virt_addr);
			};
		}
	}
}

#endif
