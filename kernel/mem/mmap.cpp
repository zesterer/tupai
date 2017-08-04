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
#include <tupai/mem/mmap.hpp>
#include <tupai/util/log.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	namespace mem
	{
		namespace mmap
		{
			static const size_t MEM_SIZE = 512 * 1024 * 1024;
			static const size_t PAGE_COUNT = MEM_SIZE / ARCH_PAGE_SIZE;
			page_t pages[PAGE_COUNT];

			void init()
			{
				// Nothing yet
			}

			int  alloc(void** phys_addr, proc::proc_ptr_t owner, uint8_t flags);
			int  reserve(void* phys_addr, proc::proc_ptr_t owner, uint8_t flags);
			int  dealloc(void* phys_addr);
			void display();
		}
	}
}
