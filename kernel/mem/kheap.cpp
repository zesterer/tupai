//
// file : kheap.cpp
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

#include <tupai/mem/kheap.hpp>
#include <tupai/util/box.hpp>
#include <tupai/util/boot.hpp>

namespace tupai::mem::kheap
{
	extern "C" char kheap_start[];
	extern "C" char kheap_end[];

	static const size_t BLOCK_SIZE = 64;

	util::Box<Pool> heap;

	void init()
	{
		heap.create(Pool::from((uintptr_t)kheap_start, (uintptr_t)kheap_end - (uintptr_t)kheap_start, BLOCK_SIZE).expect("Failed to create kernel heap"));
		util::bootlog("Kernel heap initiated");
	}

	uintptr_t alloc(size_t bytes)
	{
		return heap->alloc_bytes(bytes).expect("Kernel heap error when attempting to allocate block of size {}", bytes);
	}

	void dealloc(uintptr_t ptr)
	{
		heap->dealloc_bytes(ptr).expect("Kernel heap error when attempting to deallocate block at {}", ptr);;
	}

	void display(size_t n)
	{
		heap->display(n);
	}
}
