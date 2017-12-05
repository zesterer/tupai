//
// file : pool.hpp
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

#ifndef TUPAI_MEM_POOL_HPP
#define TUPAI_MEM_POOL_HPP

#include <tupai/util/box.hpp>
#include <tupai/util/refarr.hpp>
#include <tupai/util/result.hpp>
#include <tupai/util/status.hpp>
#include <tupai/util/math.hpp>

namespace tupai::mem
{
	enum class PoolError
	{
		NOPOOL,
		TOOSMALL,
		FULL,
		CORRUPTION,
		INVALID,
	};

	enum class PoolEntry : uint8_t
	{
		ERROR = 0b00,
		FREE  = 0b01,
		HEAD  = 0b10,
		TAIL  = 0b11,
	};

	struct Pool
	{
	private:
		static const size_t ENTRIES_PER_BYTE = 4;

		uintptr_t _heap;
		uint8_t* _map;
		size_t _blocks;
		size_t _block_sz;

		PoolEntry _get(size_t i) { return static_cast<PoolEntry>((this->_map[i / 4] >> ((i % 4) * 2)) & 0b11); }
		void _set(size_t i, PoolEntry val) { this->_map[i / 4] &= ~((uint8_t)0b11 << ((i % 4) * 2)); this->_map[i / 4] |= ((static_cast<uint8_t>(val) & 0b11) << ((i % 4) * 2)); }
		uintptr_t _get_ptr(size_t i) { return this->_heap + i * this->_block_sz; }

	public:
		Pool(uintptr_t heap, uint8_t* map, size_t blocks, size_t block_sz);

		static util::Result<Pool, PoolError> from(uintptr_t start, size_t size, size_t bs);

		util::Result<uintptr_t, PoolError> alloc(size_t bytes, size_t align = 1);
		util::Status<PoolError> dealloc(uintptr_t ptr);

		void display(size_t n = 256);

		util::Status<PoolError> check();
	};
}

#endif
