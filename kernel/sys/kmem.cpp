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
#include <tupai/sys/kmem.hpp>
#include <tupai/sys/pool.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace sys
	{
		const size_t KMEM_SIZE = 4096 * 4096; // 4096 4K pages (16M)
		const size_t KMEM_BLOCK_SIZE = 64;   // 16th of a 4K page (256B)

		pool_t kmem_pool;

		void kmem_init()
		{
			void* pool = arch_kernel_alloc(KMEM_SIZE);

			if (pool == nullptr)
				panic("Could not allocate kernel memory pool");

			bool result = pool_construct(&kmem_pool, pool, KMEM_SIZE, KMEM_BLOCK_SIZE);

			if (!result)
				panic("Could not construct kernel memory pool");
		}

		void* kmem_alloc(size_t n)
		{
			void* ptr = pool_alloc(&kmem_pool, n);

			if (ptr == nullptr)
				panic("Failed to allocate kernel blocks");

			return ptr;
		}

		void kmem_dealloc(void* ptr)
		{
			pool_dealloc(&kmem_pool, ptr);
		}

		void kmem_info()
		{
			util::print("Kernel memory info:\n",
				"  blocks     -> ", kmem_pool.block_count, '\n',
				"  pool_map   -> ", (void*)kmem_pool.map, '\n',
				"  pool_body  -> ", (void*)kmem_pool.body, '\n',
				"  block_size -> ", (void*)kmem_pool.block_size, '\n'
			);
		}

		void kmem_display()
		{
			pool_display(&kmem_pool, 256);
		}
	}
}
