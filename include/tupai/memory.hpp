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

	const byte MEMORY_PHYS_FLAG_USED   = (1 << 0); // Is this region allocated?
	const byte MEMORY_PHYS_FLAG_GPMEM  = (1 << 1); // General-purpose RAM?
	const byte MEMORY_PHYS_FLAG_STATIC = (1 << 2); // Is this memory statically used by something (i.e: kernel VBE video memory)?

	const byte MEMORY_VIRT_FLAG_USED = (1 << 0); // Is this frame in use?
	const byte MEMORY_VIRT_FLAG_SHARED = (1 << 1); // Is this frame shared by another process (not including threads)?
	const byte MEMORY_VIRT_FLAG_OWNER  = (1 << 2); // If this frame is shared, are we the owner of it?

	extern const umem MEMORY_PAGE_SIZE; // 4K - Size of one page
	extern const umem MEMORY_FRAME_PAGES; // In Pages (4K)
	extern const umem MEMORY_FRAME_SIZE;

	struct memory_virt_frame
	{
		byte flags;
		umem phys_index; // Index into the global physical memory table

		bool is_used() { return this->flags & MEMORY_PHYS_FLAG_USED; }
	};

	struct memory_phys_frame
	{
		byte flags = 0b00000000; // No flags
		uint32 pid = 0; // 0 = Kernel
		umem virt_index; // Index into the process's virtual memory table

		bool is_used() { return this->flags & MEMORY_VIRT_FLAG_USED; }
	};

	void memory_init();
	void memory_enforce(); // On most systems, this means 'enable paging'

	memory_info memory_get_info();
	bool memory_map_frame(umem phys_addr, umem virt_addr, uint32 pid, byte phys_flags = 0, byte virt_flags = 0);
	umem memory_get_size_kb();
	const memory_phys_frame* memory_get_frame(umem address);
}

#endif
