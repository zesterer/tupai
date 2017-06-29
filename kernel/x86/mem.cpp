//
// file : mem.cpp
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
#include <tupai/x86/mem.hpp>
#include <tupai/x86/multiboot.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/arch.hpp>

#include <tupai/util/out.hpp>
#include <tupai/debug.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		/*struct mem_entry_t
		{
			sys::pid_t id;
		};

		mem_entry_t* mem_map = nullptr;
		size_t       mem_pages;

		size_t mem_size;

		void memory_init()
		{
			mem_size = memory_size();
			mem_pages = util::align_floor(mem_size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE;
			//mem_map = new mem_entry_t[sizeof(mem_entry_t) * mem_pages];

			debug_println("Memory size = ", util::fmt_int<size_t>(mem_size, 16));
			debug_println("Allocating space for a ", mem_pages, "-page memory heap");

			// TODO : Fix this!!!

			for (size_t i = 0; i < mem_pages; i ++)
				mem_map[i].id = sys::NO_PROC_ID;

			// Reserve kernel memory
			//memory_reserve(0, arch_get_kernel_end(), sys::KERNEL_PROC_ID);

			// Display memory map
			memory_display();
		}

		size_t memory_size()
		{
			mb_meminfo_t meminfo = multiboot_get_meminfo();
			return (1024 + meminfo.upper) * 1024; // 1 MB high memory offset + upper memory
		}

		bool memory_reserve(size_t start, size_t size, sys::pid_t proc)
		{
			size_t spage = util::align_floor(start, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE;
			size_t epage = util::align_ceiling(start + size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE;

			for (size_t i = spage; i <= epage; i ++)
			{
				if (mem_map[i].id != sys::NO_PROC_ID)
					return false;
			}

			debug_println("Allocating pages between indexes ", util::fmt_int<size_t>(spage, 16), " and ", util::fmt_int<size_t>(epage, 16));

			for (size_t i = spage; i <= epage; i ++)
				mem_map[i].id = proc;

			return true;
		}

		void memory_display()
		{
			sys::pid_t opid = -2; // No memory map entry should be allocated to -2

			for (size_t i = 0; i < mem_pages; i ++)
			{
				sys::pid_t npid = mem_map[i].id;

				if (npid != opid)
				{
					util::println(util::fmt_int<size_t>(i * ARCH_PAGE_SIZE, 16, sizeof(void*) * 2), " = Process ", npid);
					opid = npid;
				}
			}
		}*/
	}
}
