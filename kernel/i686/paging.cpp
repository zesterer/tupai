/*
* 	file : paging.cpp
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
#include <tupai/i686/paging.hpp>
#include <tupai/i686/i686.hpp>

#include <tupai/x86_family/multiboot.hpp>

namespace tupai
{
	struct PageDirectory
	{
		uint32 entries[1024];

		uint32 getPtr()
		{
			return (uint32)(&entries[0]);
		}

		void set_table(umem index, uint32 page_table, uint16 flags)
		{
			this->entries[index] = ((page_table - KERNEL_VIRTUAL_OFFSET) & 0xFFFFF000) | ((uint32)flags & 0x00000FFF);
		}

		void clear()
		{
			for (umem i = 0; i < 1024; i ++)
				this->set_table(i, (uint32)nullptr, 0x002);
		}
	} __attribute__((__packed__));

	struct PageTable
	{
		uint32 entries[1024];

		uint32 getPtr()
		{
			return (uint32)(&entries[0]);
		}

		void set_entry(umem index, uint32 address, uint16 flags)
		{
			this->entries[index] = (address & 0xFFFFF000) | ((uint32)flags & 0x00000FFF);
		}

		void clear()
		{
			for (umem i = 0; i < 1024; i ++)
				this->set_entry(i, (uint32)nullptr, 0x002);
		}
	} __attribute__((__packed__));

	// Temporary static page directory
	PageDirectory page_directory __attribute__((aligned(4096)));
	PageTable page_tables[256] __attribute__((aligned(4096)));

	extern "C" void _load_page_directory(uint32 page_directory);
	extern "C" void _enable_paging();

	void paging_init()
	{
		// Clear the page directory
		page_directory.clear();

		// Create 1GB worth of pages for the kernel
		for (umem pt = 0; pt < 256; pt ++)
		{
			for (umem entry = 0; entry < 1024; entry ++)
				page_tables[pt].set_entry(entry, (pt * 1024 + entry) * 0x1000, 0x003);
		}

		// Identity map everything above 0xE0000000 - for Framebuffer / PCI stuff
		uint32 pos = x86_family::multiboot_get_framebuffer().address; // TOTAL HACK!!!
		for (umem pt = (pos - 0xC0000000) / 0x400000; pt < 256; pt ++)
		{
			for (umem entry = 0; entry < 1024; entry ++)
				page_tables[pt].set_entry(entry, pos | ((pt * 1024 + entry) * 0x1000), 0x003);
		}

		// Assign the 1GB of page tables to the page directory - set the 'present' bit to 1
		for (umem pt = 0; pt < 256; pt ++)
			page_directory.set_table(768 + pt, page_tables[pt].getPtr(), 0x003);
	}

	void paging_enable()
	{
		_load_page_directory(page_directory.getPtr() - KERNEL_VIRTUAL_OFFSET);
		_enable_paging();
	}
}
