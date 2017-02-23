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

#include <tupai/util/out.hpp>
#include <tupai/util/conv.hpp>
#include <tupai/util/math.hpp>
#include <tupai/util/cstr.hpp>

#include <tupai/kdebug.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	namespace x86_family
	{
		multiboot_header header;

		const umem MODULE_MAX = 16;
		const umem MODULE_MAX_STRING = 64;
		multiboot_module_info modules[MODULE_MAX];
		char module_string[MODULE_MAX_STRING + 1][MODULE_MAX];
		umem module_count = 0;

		void multiboot_init(ptr_t mb_header, uint32 mb_magic)
		{
			if (mb_magic != 0x2BADB002)
				kpanic("Multiboot magic value does not match!");

			// Extract header
			header = *((multiboot_header*)((umem)mb_header + KERNEL_VIRTUAL_OFFSET));

			// Extract modules
			module_count = util::min(MODULE_MAX, header.module_count);
			for (umem i = 0; i < module_count; i ++)
			{
				const multiboot_module_info* info = &((const multiboot_module_info*)header.module_address)[i];
				modules[i] = *info;
				util::cstr_copy((const char*)modules[i].string, module_string[i], MODULE_MAX_STRING);
				modules[i].string = (uint32)module_string[i];
			}

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

		multiboot_module_info multiboot_get_module_info(umem n)
		{
			return modules[n];
		}
	}
}
