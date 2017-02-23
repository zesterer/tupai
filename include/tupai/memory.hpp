/*
* 	file : memory.hpp
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

#ifndef TUPAI_MEMORY_HPP
#define TUPAI_MEMORY_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	struct memory_info
	{
		umem used;
		umem size;

		memory_info(umem used, umem size)
		{
			this->used = used;
			this->size = size;
		}
	};

	const byte MEMORY_FLAG_USED   = (1 << 0); // Is this region allocated?
	const byte MEMORY_FLAG_GPMEM  = (1 << 1); // General-purpose RAM?
	const byte MEMORY_FLAG_STATIC = (1 << 2); // Is this memory statically used by something (i.e: kernel VBE video memory)?

	const umem MEMORY_PAGE_SIZE = 4096; // 4K - Size of one page
	const umem MEMORY_FRAME_PAGES = 4; // In Pages (4K)
	const umem MEMORY_FRAME_SIZE = MEMORY_PAGE_SIZE * MEMORY_FRAME_PAGES;

	struct memory_virt_frame
	{
		static const byte FLAG_SHARED = (1 << 0); // Is this region shared by another process (not including threads)?
		static const byte FLAG_OWNER  = (1 << 1); // If this region is shared, are we the owner of it?

		byte flags;
		umem phys_index; // Index into the global physical memory table
	};

	struct memory_phys_frame
	{
		byte flags = 0b00000000; // No flags
		uint32 pid = 0; // 0 = Kernel
		umem virt_index; // Index into the process's virtual memory table

		bool is_used() { return this->flags & MEMORY_FLAG_USED; }
	};

	struct memory_process_map
	{
		uint32 pid; // 0 = none, 1 = kernel, 2 = processes
		umem virt_offset = 0; // For the kernel, this is 0xC0000000

		memory_virt_frame virt_frames[65536]; // 1G for now. TODO : Expand this dynamically for larger processes

		umem get_virt_addr(umem index) { return this->virt_offset + index * MEMORY_FRAME_SIZE; }
	};

	void memory_init();
	void memory_enforce(); // On most systems, this means 'enable paging'

	memory_info memory_get_info();
	bool memory_map_frame(umem address, uint32 pid, byte flags);
	umem memory_get_size_kb();
	const memory_phys_frame* memory_get_frame(umem address);
}

#endif
