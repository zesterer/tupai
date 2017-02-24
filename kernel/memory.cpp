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
#include <tupai/util/mem.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/i686.hpp>
	#include <tupai/x86_family/multiboot.hpp>
	#include <tupai/x86_family/paging.hpp>
#endif

namespace tupai
{
	static umem kernel_dyn_end;
	static umem phys_map_start;
	static umem phys_map_size;
	static umem phys_map_end;

	// Kernel end pointer
	extern "C" byte kernel_start;
	extern "C" byte kernel_end;

	const umem MEMORY_PAGE_SIZE = 4096; // 4K - Size of one page
	const umem MEMORY_FRAME_PAGES = 4; // In Pages (4K)
	const umem MEMORY_FRAME_SIZE = MEMORY_PAGE_SIZE * MEMORY_FRAME_PAGES;
	const umem MEMORY_SIZE_KB = 0x40000;
	const umem MEMORY_FRAME_COUNT = (0x40000 / MEMORY_FRAME_SIZE) * 1024;
	const umem MEMORY_PROCESS_FRAME_COUNT = (0x10000 / MEMORY_FRAME_SIZE) * 1024; // 1G (configurable)

	static ptr_t memory_early_alloc(umem size);

	struct memory_global_map
	{
		umem phys_offset = 0; // Offset of the global physical memory table in memory (usually above kernel_end)
		memory_phys_frame phys_frames[MEMORY_FRAME_COUNT]; // 4G - entire physical address space. TODO : Expand this dynamically

		umem get_phys_addr(umem index) { return this->phys_offset + index * MEMORY_FRAME_SIZE; }
	};

	struct memory_process_map
	{
		uint32 pid; // 0 = none, 1 = kernel, 2 = processes
		umem virt_offset = 0; // For the kernel, this is 0xC0000000

		memory_virt_frame virt_frames[MEMORY_PROCESS_FRAME_COUNT]; // 1G for now. TODO : Expand this dynamically for larger processes

		umem get_virt_addr(umem index) { return this->virt_offset + index * MEMORY_FRAME_SIZE; }
	};

	struct memory_map
	{
		memory_global_map global_map;
		memory_process_map process_maps[16]; // Allow space for 16 processes for now

		memory_map() {}
	};

	static memory_map* g_memory_map;
	static bool memory_enforced = false;

	void memory_init()
	{
		#if defined(SYSTEM_ARCH_i686)
			kernel_dyn_end = util::align_ceiling((umem)&kernel_end - KERNEL_VIRTUAL_OFFSET, util::MEM_ALIGN_KB);

			x86_family::paging_init();
		#endif

		// Get space for the memory map and call the constructor
		g_memory_map = (memory_map*)memory_early_alloc(sizeof(memory_map));
		new (g_memory_map) memory_map(); // TODO : Use an early 1G paging map with ASM to allow this
	}

	void memory_enforce()
	{
		memory_enforced = true;

		// Get the Multiboot memory information
		x86_family::multiboot_header mbh = x86_family::multiboot_get_header();

		// Find the limits of the available memory map
		phys_map_start = util::align_ceiling(kernel_dyn_end, util::MEM_ALIGN_KB);
		phys_map_end = util::align_floor(mbh.mem_upper * 1024, util::MEM_ALIGN_KB);
		phys_map_size = util::align_floor(phys_map_end - phys_map_start, util::MEM_ALIGN_KB);

		// Map default kernel memory
		for (umem addr = 0x0; addr < kernel_dyn_end; addr += MEMORY_FRAME_SIZE)
			memory_map_frame(addr, 0, 0x0);

		#if defined(SYSTEM_ARCH_i686)
			x86_family::paging_enable();
		#endif
	}

	memory_info memory_get_info()
	{
		return memory_info(0, phys_map_size);
	}

	static ptr_t memory_early_alloc(umem size)
	{
		size = util::align_ceiling(size, util::MEM_ALIGN_KB);
		ptr_t ptr = (ptr_t)(kernel_dyn_end + KERNEL_VIRTUAL_OFFSET);
		kernel_dyn_end += size;
		return ptr;
	}

	bool memory_map_frame(umem phys_addr, umem virt_addr, uint32 pid, byte phys_flags, byte virt_flags)
	{
		umem phys_index = util::align_floor(phys_addr, MEMORY_FRAME_SIZE) / MEMORY_FRAME_SIZE;
		memory_phys_frame* phys_frame = &g_memory_map->global_map.phys_frames[phys_index];

		umem virt_index = util::align_floor(virt_addr - g_memory_map->process_maps[pid].virt_offset, MEMORY_FRAME_SIZE) / MEMORY_FRAME_SIZE;
		memory_virt_frame* virt_frame = &g_memory_map->process_maps[pid].virt_frames[virt_index];

		if (phys_frame->is_used() || virt_frame->is_used())
			return false;
		else
		{
			phys_frame->flags = phys_flags | MEMORY_PHYS_FLAG_USED;
			phys_frame->pid = pid;
			virt_frame->phys_index = phys_index;
			virt_frame->flags = virt_flags | MEMORY_VIRT_FLAG_USED;
			return true;
		}
	}

	const memory_phys_frame* memory_get_frame(umem address)
	{
		umem index = util::align_floor(address, MEMORY_FRAME_SIZE) / MEMORY_FRAME_SIZE;
		return &g_memory_map->global_map.phys_frames[index];
	}

	umem memory_get_size_kb()
	{
		return 0x400000; // 4GB
	}
}
