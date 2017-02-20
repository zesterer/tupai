/*
* 	file : boot_paging.hpp
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

#ifndef TUPAI_I686_BOOT_BOOT_PAGING_HPP
#define TUPAI_I686_BOOT_BOOT_PAGING_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	namespace i686
	{
		extern "C" volatile uint32 boot_page_directory[1024] __attribute__((section(".data.boot"))) __attribute__((aligned(4096)));
		extern volatile uint32 boot_page_tables[1024][256] __attribute__((section(".data.boot"))) __attribute__((aligned(4096)));

		extern "C" void init_boot_paging() __attribute__((section(".text.boot")));
	}
}

#endif
