//
// file : mmap.cpp
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
#include <tupai/sys/mmap.hpp>
#include <tupai/arch.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/mutex.hpp>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/multiboot.hpp>
#endif

namespace tupai
{
	namespace sys
	{
		static const int MMAP_BR_P2 = 8;
		static const int MMAP_BR    = 1 << MMAP_BR_P2;

		util::mutex mmap_mutex;

		struct mmap_br_t;

		struct mmap_node_t
		{
			uint8_t depth;
			mmap_status_t status = mmap_status_t(NO_PROC_ID, false);

			mmap_br_t* br = nullptr;

			mmap_node_t() {}
			mmap_node_t(uint8_t depth, mmap_status_t status)
			{
				this->depth = depth;
				this->status = status;
				this->br = nullptr;
			}
			~mmap_node_t();
		};

		struct mmap_br_t
		{
			mmap_node_t nodes[MMAP_BR];

			mmap_br_t(int depth, mmap_status_t status)
			{
				for (size_t i = 0; i < MMAP_BR; i ++)
				{
					this->nodes[i].depth = depth;
					this->nodes[i].status = status;
					this->nodes[i].br = nullptr;
				}
			}
		};

		mmap_node_t::~mmap_node_t()
		{
			delete this->br;
		}

		mmap_node_t root(3, mmap_status_t(NO_PROC_ID, false));

		void mmap_init()
		{
			mmap_mutex.lock(); // Begin critical section

			root = mmap_node_t(3, mmap_status_t(NO_PROC_ID, false));

			//root.br = new mmap_br_t();
			//root.br->depth = 2;//util::align_ceiling(sizeof(size_t) * 8 - ARCH_PAGE_SIZE_P2, MMAP_BR_P2) / MMAP_BR_P2 - 1;

			mmap_mutex.unlock(); // End critical section

			//mmap_reserve(0, 8192, KERNEL_PROC_ID);

			#if defined(ARCH_FAMILY_x86)
				x86::mb_meminfo_t meminfo = x86::multiboot_get_meminfo();
				mmap_reserve(1024 * 1024, meminfo.upper * 1024, NO_PROC_ID); // 1 MB to N KB
			#endif
		}

		void __mmap_reserve(mmap_node_t* node, uint64_t addr, mmap_status_t status, uint64_t start, uint64_t size)
		{
			uint64_t nsize = 1 << (node->depth * MMAP_BR_P2);

			if (
				(start > addr && start < addr + nsize) ||
				(start + size > addr && start + size < addr + nsize)
			)
			{
				if (node->depth > 0 && node->br == nullptr)
					node->br = new mmap_br_t(node->depth - 1, node->status);
			}

			if (node->br != nullptr)
			{
				node->status.owner = INVALID_PROC_ID;

				for (size_t i = 0; i < MMAP_BR; i ++)
				{
					__mmap_reserve(&node->br->nodes[i], addr, status, start, size);
					addr += 1 << ((node->depth - 1) * MMAP_BR_P2);
				}
			}
			else if (start <= addr && start + size >= addr + nsize)
			{
				node->status = status;
			}

			/*
			size_t node_size = (1 << (depth * MMAP_BR_P2));

			if (addr >= start && addr < start + size)
				node->status = status;

			if (node->br == nullptr)
			{
				if (depth <= 0)
					return;

				if (start < addr + node_size && start + size > addr)
				{
					node->br = new mmap_br_t();
					node->br->depth = depth - 1;
				}
			}
			else
			{
				// TODO : Re-add this
				//if (addr >= start && addr + node_size <= start + size)
				//{
				//	delete node->br;
				//	node->br = nullptr;
				//	return;
				//}
			}

			if (node->br != nullptr)
			{
				for (int i = 0; i < MMAP_BR; i ++)
				{
					__mmap_reserve(&node->br->nodes[i], depth - 1, addr, status, start, size);
					addr += 1 << (node->br->depth * MMAP_BR_P2);
				}
			}*/
		}

		void mmap_reserve(uint64_t start, uint64_t size, pid_t owner)
		{
			mmap_mutex.lock(); // Begin critical section

			uint64_t pstart = start / ARCH_PAGE_SIZE;
			uint64_t psize = util::align_ceiling(start + size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE - pstart;

			__mmap_reserve(&root, 0, mmap_status_t(owner, true), pstart, psize);

			mmap_mutex.unlock(); // End critical section
		}

		mmap_status_t __mmap_display(mmap_node_t* node, uint64_t addr, mmap_status_t cstatus)
		{
			if (node->br != nullptr)
			{
				for (int i = 0; i < MMAP_BR; i ++)
				{
					cstatus = __mmap_display(&node->br->nodes[i], addr, cstatus);
					addr += 1 << ((node->depth - 1) * MMAP_BR_P2);
				}

				return cstatus;
			}
			else
			{
				if (node->status != cstatus)
				{
					cstatus = node->status;
					util::println((void*)(addr * ARCH_PAGE_SIZE), " : owner = ", proc_get_name(cstatus.owner), ", valid = ", cstatus.valid > 0);
				}

				return cstatus;
			}
		}

		void mmap_display()
		{
			mmap_mutex.lock(); // Begin critical section

			util::println("--- Physical Memory Map (1 page = ", util::fmt_int<uint16_t>(ARCH_PAGE_SIZE, 16), ") ---");
			__mmap_display(&root, 0, mmap_status_t(INVALID_PROC_ID, false));

			mmap_mutex.unlock(); // End critical section
		}
	}
}
