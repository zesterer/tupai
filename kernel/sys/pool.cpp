//
// file : pool.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/sys/pool.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/out.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	namespace sys
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

		bool pool_construct(pool_t* pool, void* start, size_t size, size_t block_size)
		{
			pool->mutex.lock(); // Begin critical section

			pool->block_size  = block_size;
			pool->body        = util::align_ceiling((size_t)start, 16); // Align to 16 bytes
			pool->block_count = (4 * ((uint64_t)size + (size_t)start - pool->body)) / (4 * (uint64_t)pool->block_size + 1);
			pool->map         = pool->body + pool->block_size * pool->block_count;

			// Clear the map
			uint8_t* map = (uint8_t*)pool->map;
			for (size_t i = 0; i < pool->block_count; i ++)
			{
				map[i] =
					((uint8_t)block_status::UNUSED << 0) |
					((uint8_t)block_status::UNUSED << 2) |
					((uint8_t)block_status::UNUSED << 4) |
					((uint8_t)block_status::UNUSED << 6);
			}

			bool success = (pool->block_count > 0);

			pool->mutex.unlock(); // End critical section

			return success;
		}

		void* pool_alloc(pool_t* pool, size_t n)
		{
			pool->mutex.lock(); // Begin critical section

			size_t start = 0;
			size_t blocks_needed = util::align_ceiling(n, pool->block_size) / pool->block_size;

			bool success = false;
			for (size_t i = 0; i < pool->block_count; i ++)
			{
				if (pool_get(pool, i) == block_status::UNUSED) // Found an unused block! keep searching to check size
				{
					start = i;

					bool tosmall = false;
					for (; i < start + blocks_needed; i ++)
					{
						if (i == pool->block_count)
							panic("Cannot find space to allocate kernel memory");

						if (pool_get(pool, i) != block_status::UNUSED)
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
				pool_set(pool, start, block_status::HEAD);
				for (size_t i = 1; i < blocks_needed; i ++)
					pool_set(pool, start + i, block_status::TAIL);

				mem = (void*)(pool->body + start * pool->block_size);
			}

			pool->mutex.unlock(); // End critical section
			return mem;
		}

		void pool_dealloc(pool_t* pool, void* ptr)
		{
			pool->mutex.lock(); // Begin critical section

			size_t index = (size_t)ptr - pool->body;

			if (index % pool->block_size != 0)
				panic("Attempting to deallocate invalid unaligned pointer");
			else
				index /= pool->block_size;

			if (pool_get(pool, index) != block_status::HEAD)
				panic("Attempting to deallocate invalid unallocated pointer");

			pool_set(pool, index, block_status::UNUSED);
			for (size_t i = index + 1; pool_get(pool, i) == block_status::TAIL && i < pool->block_count; i ++)
				pool_set(pool, i, block_status::UNUSED);

			pool->mutex.unlock(); // End critical section
		}

		void pool_display(pool_t* pool, size_t n)
		{
			util::println("--- Memory pool map ---");

			size_t max = 0;
			for (size_t i = 0; i < n; i ++)
			{
				switch (pool_get(pool, i))
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
				switch(pool_get(pool, i))
				{
				case block_status::HEAD:
					util::print('H');
					break;
				case block_status::TAIL:
					util::print('T');
					break;
				case block_status::UNUSED:
					util::print('.');
					break;
				default:
					util::print('?');
					break;
				}
			}
			util::print('\n');
		}

		block_status pool_get(pool_t* pool, size_t index)
		{
			size_t byte_index = index / 4;
			size_t bit_offset = (index % 4) * 2;

			return (block_status)((((uint8_t*)pool->map)[byte_index] >> bit_offset) & 0b11);
		}

		void pool_set(pool_t* pool, size_t index, block_status status)
		{
			size_t byte_index  = index / 4;
			size_t bit_offset = (index % 4) * 2;

			uint8_t byte = ((uint8_t*)pool->map)[byte_index];

			byte &= ~(0b11 << bit_offset);
			byte |= ((uint8_t)status << bit_offset);

			((uint8_t*)pool->map)[byte_index] = byte;
		}
	}
}
