//
// file : pool.hpp
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

#ifndef TUPAI_MEM_POOL_HPP
#define TUPAI_MEM_POOL_HPP

// Tupai
#include <tupai/util/mutex.hpp>
#include <tupai/util/spinlock.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace mem
	{
		namespace pool
		{
			struct pool_t
			{
				size_t _map;
				size_t _body;
				size_t _block_size;
				size_t _block_count;
				util::spinlock_t _spinlock;

				void* alloc(size_t n);
				void  dealloc(void* ptr);
				void  display(size_t n = 32);
			};

			bool create(pool_t* pool, void* start, size_t size, size_t block_size = 64);
		}
	}
}

#endif
