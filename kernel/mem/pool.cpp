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

// Tupai
#include <tupai/mem/pool.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/log.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	namespace mem
	{
		namespace pool
		{
			enum class block_status : uint8_t
			{
				ERROR  = 0b00,
				UNUSED = 0b01,
				TAIL   = 0b10,
				HEAD   = 0b11,
			};

			static block_status pool_get(pool_t* pool, size_t index);
			static void pool_set(pool_t* pool, size_t index, block_status status);

			bool create(pool_t* pool, void* start, size_t size, size_t block_size)
			{
				pool->_spinlock.lock(); // Begin critical section

				size_t diff = (size_t)start - util::align_ceiling((size_t)start, 16);
				start = (void*)((size_t)start + diff);
				size -= diff;

				pool->_block_size  = block_size;
				pool->_body        = (size_t)start; // Align to 16 bytes
				pool->_block_count = (4 * ((uint64_t)size + (size_t)start - pool->_body)) / (4 * (uint64_t)pool->_block_size + 1);
				pool->_map         = pool->_body + pool->_block_size * pool->_block_count;

				// Clear the map
				uint8_t* map = (uint8_t*)pool->_map;
				for (size_t i = 0; i < pool->_block_count; i ++)
				{
					map[i] =
						((uint8_t)block_status::UNUSED << 0) |
						((uint8_t)block_status::UNUSED << 2) |
						((uint8_t)block_status::UNUSED << 4) |
						((uint8_t)block_status::UNUSED << 6);
				}

				bool success = (pool->_block_count > 0);

				pool->_spinlock.unlock(); // End critical section

				return success;
			}

			void* pool_t::alloc(size_t n, size_t align)
			{
				this->_spinlock.lock(); // Begin critical section

				size_t start = 0;
				size_t blocks_needed = util::align_ceiling(n, this->_block_size) / this->_block_size;

				bool success = false;
				for (size_t i = 0; i < this->_block_count; i ++)
				{
					if (pool_get(this, i) == block_status::UNUSED && (size_t)(this->_body + i * this->_block_size) % align == 0) // Found an unused block! keep searching to check size
					{
						start = i;

						bool tosmall = false;
						for (; i < start + blocks_needed; i ++)
						{
							if (i == this->_block_count)
								panic("Cannot find space to allocate kernel memory");

							if (pool_get(this, i) != block_status::UNUSED)
							{
								tosmall = true;
								break;
							}
						}

						if (!tosmall)
						{
							success = true;
							break;
						}
					}
				}

				void* mem = nullptr;

				if (success)
				{
					pool_set(this, start, block_status::HEAD);
					for (size_t i = 1; i < blocks_needed; i ++)
						pool_set(this, start + i, block_status::TAIL);

					mem = (void*)(this->_body + start * this->_block_size);
				}

				this->_spinlock.unlock(); // End critical section
				return mem;
			}

			void pool_t::dealloc(void* ptr)
			{
				this->_spinlock.lock(); // Begin critical section

				size_t index = (size_t)ptr - this->_body;

				if (index % this->_block_size == 0)
				{
					index /= this->_block_size;

					if (pool_get(this, index) == block_status::HEAD)
					{
						pool_set(this, index, block_status::UNUSED);
						for (size_t i = index + 1; pool_get(this, i) == block_status::TAIL && i < this->_block_count; i ++)
							pool_set(this, i, block_status::UNUSED);
					}
					else
						panic("Attempting to deallocate invalid unallocated pointer ", ptr);
				}
				else
					panic("Attempting to deallocate invalid unaligned pointer ", ptr);

				this->_spinlock.unlock(); // End critical section
			}

			void pool_t::display(size_t n)
			{
				util::logln("--- Memory pool map ---");

				size_t max = 0;
				for (size_t i = 0; i < n; i ++)
				{
					switch (pool_get(this, i))
					{
					case block_status::HEAD:
					case block_status::TAIL:
						max = i;
						break;
					default:
						break;
					}
				}

				for (size_t i = 0; i < n && i < max; i ++)
				{
					switch(pool_get(this, i))
					{
					case block_status::HEAD:
						util::log('H');
						break;
					case block_status::TAIL:
						util::log('T');
						break;
					case block_status::UNUSED:
						util::log('.');
						break;
					default:
						util::log('?');
						break;
					}
				}
				util::log('\n');
			}

			block_status pool_get(pool_t* pool, size_t index)
			{
				size_t byte_index = index / 4;
				size_t bit_offset = (index % 4) * 2;

				return (block_status)((((uint8_t*)pool->_map)[byte_index] >> bit_offset) & 0b11);
			}

			void pool_set(pool_t* pool, size_t index, block_status status)
			{
				size_t byte_index  = index / 4;
				size_t bit_offset = (index % 4) * 2;

				uint8_t byte = ((uint8_t*)pool->_map)[byte_index];

				byte &= ~(0b11 << bit_offset);
				byte |= ((uint8_t)status << bit_offset);

				((uint8_t*)pool->_map)[byte_index] = byte;
			}
		}
	}
}
