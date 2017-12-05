//
// file : pool.cpp
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

#include <tupai/mem/pool.hpp>
#include <tupai/mem/virt.hpp>
#include <tupai/util/log.hpp>

namespace tupai::mem
{
	Pool::Pool(uintptr_t heap, uint8_t* map, size_t blocks, size_t block_sz) : _heap(heap), _map(map), _blocks(blocks), _block_sz(block_sz)
	{
		for (size_t i = 0; i < this->_blocks; i ++)
			this->_set(i, PoolEntry::FREE);
	}

	util::Result<Pool, PoolError> Pool::from(uintptr_t start, size_t size, size_t bs)
	{
		uintptr_t nheap = util::ceil(start, virt::PAGE_SIZE);
		size_t nsize = size - (nheap - start);
		size_t bc = (nsize * ENTRIES_PER_BYTE) / (bs * ENTRIES_PER_BYTE + 1);
		uintptr_t nmap = (nheap + nsize) - util::ceil(bc, ENTRIES_PER_BYTE) / ENTRIES_PER_BYTE;

		if (bc == 0)
			return util::Result<Pool, PoolError>(PoolError::TOOSMALL);
		else
			return util::Result<Pool, PoolError>(Pool(nheap, reinterpret_cast<uint8_t*>(nmap), bc, bs));
	}

	util::Result<uintptr_t, PoolError> Pool::alloc(size_t bytes, size_t align)
	{
		size_t blocks = util::ceil(bytes, this->_block_sz) / this->_block_sz;

		for (size_t i = 0; i < this->_blocks; i ++)
		{
			PoolEntry type = this->_get(i);
			if (type == PoolEntry::ERROR)
				return util::Result<uintptr_t, PoolError>(PoolError::CORRUPTION);
			else if (type == PoolEntry::FREE && this->_get_ptr(i) % align == 0)
				for (size_t j = i; j < this->_blocks; j ++)
				{
					PoolEntry type = this->_get(i);
					if (type == PoolEntry::ERROR)
						return util::Result<uintptr_t, PoolError>(PoolError::CORRUPTION);
					else if (j - i == blocks)
					{
						this->_set(i, PoolEntry::HEAD);
						for (size_t k = i + 1; k < j; k ++)
							this->_set(k, PoolEntry::TAIL);

						return util::Result<uintptr_t, PoolError>(this->_get_ptr(i));
					}
					else if (type != PoolEntry::FREE)
					{
						i = j;
						break;
					}
				}
		}

		return util::Result<uintptr_t, PoolError>(PoolError::FULL);
	}

	util::Status<PoolError> Pool::dealloc(uintptr_t ptr)
	{
		if ((ptr - this->_heap) % this->_block_sz != 0 ||
			ptr < this->_heap ||
			ptr > this->_heap + this->_blocks * this->_block_sz)
			return util::Status<PoolError>(PoolError::INVALID);

		size_t i = (ptr - this->_heap) / this->_block_sz;
		PoolEntry type = this->_get(i);
		if (type == PoolEntry::ERROR)
			return util::Status<PoolError>(PoolError::CORRUPTION);
		else if (type == PoolEntry::HEAD)
		{
			this->_set(i, PoolEntry::FREE);
			for (i ++; i < this->_blocks; i ++)
			{
				PoolEntry type = this->_get(i);
				if (type == PoolEntry::ERROR)
					return util::Status<PoolError>(PoolError::CORRUPTION);
				else if (type != PoolEntry::TAIL)
					break;
				else
					this->_set(i, PoolEntry::FREE);
			}
		}
		else
			return util::Status<PoolError>(PoolError::INVALID);

		return util::Status<PoolError>();
	}

	void Pool::display(size_t n)
	{
		for (size_t i = 0; i < this->_blocks && i < n; i ++)
		{
			switch (this->_get(i))
			{
			case PoolEntry::HEAD:
					util::logc('H');
					break;
				case PoolEntry::TAIL:
					util::logc('T');
					break;
				case PoolEntry::FREE:
					util::logc('-');
					break;
				default:
					util::logc('!');
					break;
			}
		}

		util::logc('\n');
	}
}
