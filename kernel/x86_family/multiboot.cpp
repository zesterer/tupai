/*
* 	file : multiboot.cpp
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
#include <tupai/x86_family/multiboot.hpp>
#include <tupai/i686/i686.hpp>

#include <tupai/kdebug.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	namespace x86_family
	{
		multiboot_header header;

		void multiboot_init(ptr_t mb_header, uint32 mb_magic)
		{
			header = *((multiboot_header*)((umem)mb_header + KERNEL_VIRTUAL_OFFSET));

			klog_init("Initiated Multiboot");
		}

		multiboot_header multiboot_get_header()
		{
			return header;
		}

		multiboot_header::framebuffer multiboot_get_framebuffer()
		{
			return header.fb;
		}
	}
}
