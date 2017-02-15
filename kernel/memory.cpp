/*
* 	file : memory.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/memory.hpp>
#include <tupai/tty.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/i686.hpp>
	#include <tupai/x86_family/multiboot.hpp>
	#include <tupai/x86_family/paging.hpp>
#endif

namespace tupai
{
	const umem MEMORY_PAGE_SIZE = 4096; // 4K - Size of one page
	const umem MEMORY_REGION_PAGES = 4; // In Pages (4K)
	const umem MEMORY_REGION_SIZE = MEMORY_PAGE_SIZE * MEMORY_REGION_PAGES;

	struct memory_virt_region
	{
		static const byte FLAG_SHARED = (1 << 0); // Is this region shared by another process (not including threads)?
		static const byte FLAG_OWNER  = (1 << 1); // If this region is shared, are we the owner of it?

		byte flags;
		umem phys_index; // Index into the global physical memory table
	};

	struct memory_phys_region
	{
		static const byte FLAG_USED   = (1 << 0); // Is this region allocated?
		static const byte FLAG_GPMEM  = (1 << 1); // General-purpose RAM?
		static const byte FLAG_STATIC = (1 << 2); // Is this memory statically used by something (i.e: kernel VBE video memory)?

		byte flags = 0b00000000; // No flags
		uint32 pid; // 0 = Kernel
		umem virt_index; // Index into the process's virtual memory table
	};

	struct memory_process_map
	{
		uint32 pid; // 0 = none, 1 = kernel, 2 = processes
		umem virt_offset = 0; // For the kernel, this is 0xC0000000

		memory_virt_region virt_regions[65536]; // 1G for now. TODO : Expand this dynamically for larger processes

		umem get_virt_addr(umem index) { return this->virt_offset + index * MEMORY_REGION_SIZE; }
	};

	struct memory_global_map
	{
		umem phys_offset = 0; // Offset of the global physical memory table in memory (usually above kernel_end)
		memory_phys_region phys_regions[262144]; // 4G - entire physical address space. TODO : Expand this dynamically

		umem get_phys_addr(umem index) { return this->phys_offset + index * MEMORY_REGION_SIZE; }
	};

	struct memory_map
	{
		memory_global_map global_map;
		memory_process_map process_map[16]; // Allow space for 16 processes for now
	};

	static umem phys_heap_start;
	static umem phys_heap_size;
	static umem phys_heap_end;

	// Kernel end pointer
	extern "C" byte kernel_start;
	extern "C" byte kernel_end;

	void memory_init()
	{
		#if defined(SYSTEM_ARCH_i686)
			// Get the memory size
			x86_family::multiboot_header mbh = x86_family::multiboot_get_header();

			phys_heap_start = mbh.mem_lower * 1024 + ((umem)&kernel_end - KERNEL_VIRTUAL_OFFSET);
			phys_heap_end = mbh.mem_upper * 1024;
			phys_heap_size = phys_heap_end - phys_heap_start;

			x86_family::paging_init();
		#endif
	}

	void memory_enforce()
	{
		#if defined(SYSTEM_ARCH_i686)
			x86_family::paging_enable();
		#endif
	}

	memory_info memory_get_info()
	{
		return memory_info(0, phys_heap_size);
	}
}
