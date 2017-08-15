//
// file : kmem.cpp
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
#include <tupai/mem/mmap.hpp>
#include <tupai/util/spinlock.hpp>
#include <tupai/util/log.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	namespace mem
	{
		namespace mmap
		{
			extern "C" char start_mmap[];
			extern "C" char end_mmap[];

			// Unions are used here SPECIFICALLY to avoid global construction!
			union PC { uint64_t val; PC() {} } PAGE_COUNT;
			union MS { uint64_t val; MS() {} } MEM_SIZE;
			union Pages { page_t* val; Pages() {} } pages;

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

				PAGE_COUNT.val = (uint64_t)((size_t)end_mmap - (size_t)start_mmap) / sizeof(page_t);
				MEM_SIZE.val = PAGE_COUNT.val * ARCH_PAGE_SIZE;
				pages.val = (page_t*)start_mmap;

				for (size_t i = 0; i < PAGE_COUNT.val; i ++)
				{
					pages.val[i]._owner = ID_INVALID;
					pages.val[i]._flags = (uint8_t)page_flags::NONE;
				}

				mutex.unlock(); // End critical section
			}

			int alloc(void** phys_addr, task::proc_ptr_t owner, uint8_t flags)
			{
				mutex.lock(); // Begin critical section

				int err = 1;
				for (size_t i = 0; i < PAGE_COUNT.val; i ++)
				{
					if (pages.val[i]._owner == ID_INVALID)
					{
						pages.val[i]._owner = owner;
						pages.val[i]._flags = flags;
						*phys_addr = index_to_physical(i);
						err = 0;
						break;
					}
				}

				if (err == 1)
					panic("Out of memory (when attempting to allocate a page)");

				mutex.unlock(); // End critical section
				return err;
			}

			int alloc_contiguous(void** phys_addr, size_t size, task::proc_ptr_t owner, uint8_t flags)
			{
				mutex.lock(); // Begin critical section

				size_t page_num = util::align_ceiling(size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE;

				int err = 1;
				for (size_t i = 0; i + page_num <= PAGE_COUNT.val; i ++)
				{
					// Search for empty block
					bool failed = false;
					for (size_t j = 0; j < page_num; j ++)
					{
						if (pages.val[i + j]._owner != ID_INVALID)
							failed = true;
					}

					if (failed)
						continue;

					for (size_t j = 0; j < page_num && i + j < PAGE_COUNT.val; j ++)
					{
						pages.val[i + j]._owner = owner;
						pages.val[i + j]._flags = flags;
					}

					*phys_addr = index_to_physical(i);
					err = 0;
					break;
				}

				if (err == 1)
					panic("Out of memory (when attempting to allocate contiguous region)", PAGE_COUNT.val, " : ", page_num);

				mutex.unlock(); // End critical section
				return err;
			}

			int reserve(void* phys_addr, task::proc_ptr_t owner, uint8_t flags)
			{
				mutex.lock(); // Begin critical section

				size_t index = physical_to_index(phys_addr);

				int err = 1;
				if (pages.val[index]._owner == ID_INVALID)
				{
					pages.val[index]._owner = owner;
					pages.val[index]._flags = flags;
					err = 0;
				}

				mutex.unlock(); // End critical section
				return err;
			}

			int reserve_region(void* phys_addr, size_t size, task::proc_ptr_t owner, uint8_t flags)
			{
				size_t sindex = physical_to_index(phys_addr);
				size_t eindex = physical_to_index((void*)((size_t)phys_addr + size - 1));

				mutex.lock(); // Begin critical section

				int err = 0;
				for (size_t i = sindex; i <= eindex; i ++)
				{
					if (pages.val[i]._owner != ID_INVALID)
						err = 1;
				}

				if (err == 0)
				{
					for (size_t i = sindex; i <= eindex; i ++)
					{
						pages.val[i]._owner = owner;
						pages.val[i]._flags = flags;
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
				if (pages.val[index]._owner != ID_INVALID)
				{
					pages.val[index]._owner = ID_INVALID;
					pages.val[index]._flags = (uint8_t)page_flags::NONE;
					err = 0;
				}

				mutex.unlock(); // End critical section
				return err;
			}

			void display()
			{
				task::proc_ptr_t cproc = ID_INVALID;
				uint8_t cflags = 0x0;
				for (size_t i = 0; i < PAGE_COUNT.val; i ++)
				{
					task::proc_ptr_t nproc = pages.val[i]._owner;
					uint8_t nflags = pages.val[i]._flags;

					if (cproc != nproc || cflags != nflags || i == 0)
					{
						char name[task::PROC_NAME_MAX] = "FREE";
						nproc.get_name(name, task::PROC_NAME_MAX);

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
