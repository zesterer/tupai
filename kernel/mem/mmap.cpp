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
#include <tupai/util/spinlock.hpp>
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

			util::spinlock_t mutex;

			static void* index_to_physical(size_t index)
			{
				return (void*)(index * ARCH_PAGE_SIZE);
			}

			static size_t physical_to_index(void* phys)
			{
				return (size_t)phys / ARCH_PAGE_SIZE;
			}

			void init()
			{
				mutex.lock(); // Begin critical section

				for (size_t i = 0; i < PAGE_COUNT; i ++)
				{
					pages[i]._owner = ID_INVALID;
					pages[i]._flags = (uint8_t)page_flags::NONE;
				}

				mutex.unlock(); // End critical section
			}

			int alloc(void** phys_addr, proc::proc_ptr_t owner, uint8_t flags)
			{
				mutex.lock(); // Begin critical section

				int err = 1;
				for (size_t i = 0; i < PAGE_COUNT; i ++)
				{
					if (pages[i]._owner == ID_INVALID)
					{
						pages[i]._owner = owner;
						pages[i]._flags = flags;
						*phys_addr = index_to_physical(i);
						err = 0;
						break;
					}
				}

				if (err == 1)
					panic("Out of memory");

				mutex.unlock(); // End critical section
				return err;
			}

			int reserve(void* phys_addr, proc::proc_ptr_t owner, uint8_t flags)
			{
				mutex.lock(); // Begin critical section

				size_t index = physical_to_index(phys_addr);

				int err = 1;
				if (pages[index]._owner == ID_INVALID)
				{
					pages[index]._owner = owner;
					pages[index]._flags = flags;
					err = 0;
				}

				mutex.unlock(); // End critical section
				return err;
			}

			int reserve_region(void* phys_addr, size_t size, proc::proc_ptr_t owner, uint8_t flags)
			{
				size_t sindex = physical_to_index(phys_addr);
				size_t eindex = physical_to_index((void*)((size_t)phys_addr + size - 1));

				mutex.lock(); // Begin critical section

				int err = 0;
				for (size_t i = sindex; i <= eindex; i ++)
				{
					if (pages[i]._owner != ID_INVALID)
						err = 1;
				}

				if (err == 0)
				{
					for (size_t i = sindex; i <= eindex; i ++)
					{
						pages[i]._owner = owner;
						pages[i]._flags = flags;
					}
				}

				mutex.unlock(); // End critical section
				return err;
			}

			int dealloc(void* phys_addr)
			{
				size_t index = physical_to_index(phys_addr);

				mutex.lock(); // Begin critical section

				int err = 1;
				if (pages[index]._owner != ID_INVALID)
				{
					pages[index]._owner = ID_INVALID;
					pages[index]._flags = (uint8_t)page_flags::NONE;
					err = 0;
				}

				mutex.unlock(); // End critical section
				return err;
			}

			void display()
			{
				proc::proc_ptr_t cproc = ID_INVALID;
				uint8_t cflags = 0x0;
				for (size_t i = 0; i < PAGE_COUNT; i ++)
				{
					proc::proc_ptr_t nproc = pages[i]._owner;
					uint8_t nflags = pages[i]._flags;

					if (cproc != nproc || cflags != nflags || i == 0)
					{
						char name[proc::PROC_NAME_MAX] = "FREE";
						nproc.get_name(name, proc::PROC_NAME_MAX);

						util::log("[", index_to_physical(i), "] -> ", name, " (", nproc.id, ")");
						if (nflags & (uint8_t)page_flags::STATIC)
							util::log(" static");
						util::log('\n');

						cproc = nproc;
						cflags = nflags;
					}
				}
			}
		}
	}
}
