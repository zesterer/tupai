//
// file : kmem.cpp
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
#include <tupai/mem/kmem.hpp>
#include <tupai/mem/mmap.hpp>
#include <tupai/mem/pool.hpp>
#include <tupai/task/task.hpp>
#include <tupai/util/hwlock.hpp>
#include <tupai/util/log.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	namespace mem
	{
		namespace kmem
		{
			static const size_t KMEM_SIZE = 4096 * 4096; // 4096 4K pages (16M)
			static const size_t KMEM_BLOCK_SIZE = 32;

			static pool::pool_t pool;

			void init()
			{
				util::hwlock_acquire(); // Begin critical section

				//void* pool_area = arch_get_kernel_heap();
				//size_t pool_size = arch_get_kernel_heap_size();
				void* pool_area = nullptr; // = arch_kernel_alloc(KMEM_SIZE);
				//util::logln("AREA = ", arch_get_kernel_heap(), " SIZE = ", arch_get_kernel_heap_size());
				int alloc_result = mmap::alloc_contiguous(&pool_area, KMEM_SIZE, task::get_kernel(), 0b00000000);
				//pool_area = (void*)((size_t)pool_area + arch_get_offset());

				if (pool_area == nullptr || alloc_result != 0)
					panic("Could not allocate kernel memory pool");

				bool result = pool::create(&pool, pool_area, KMEM_SIZE, KMEM_BLOCK_SIZE);

				if (!result)
					panic("Could not construct kernel memory pool");

				util::hwlock_release(); // End critical section
			}

			void* alloc(size_t n)
			{
				util::hwlock_acquire(); // Begin critical section

				void* ptr = pool.alloc(n);

				if (ptr == nullptr)
					panic("Failed to allocate kernel memory pool block(s)");

				util::hwlock_release(); // End critical section
				return ptr;
			}

			void dealloc(void* ptr)
			{
				util::hwlock_acquire(); // Begin critical section

				pool.dealloc(ptr);

				util::hwlock_release(); // End critical section
			}

			void info()
			{
				util::log("Kernel memory info:\n",
					"  blocks     -> ", (long)pool._block_count, '\n',
					"  pool_map   -> ", (void*)pool._map, '\n',
					"  pool_body  -> ", (void*)pool._body, '\n',
					"  block_size -> ", (void*)pool._block_size, '\n'
				);
			}

			void log()
			{
				util::hwlock_acquire(); // Begin critical section

				pool.display(KMEM_SIZE / KMEM_BLOCK_SIZE);

				util::hwlock_release(); // End critical section
			}
		}
	}
}
