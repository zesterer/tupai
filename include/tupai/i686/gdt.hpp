/*
* 	file : gdt.hpp
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

#ifndef TUPAI_I686_GDT_HPP
#define TUPAI_I686_GDT_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	const umem GDT_SIZE = 3;

	void gdt_init();
	void gdt_install();
	//extern "C" void gdt_install(void* ptr, umem size);
	void gdt_set_entry(umem n, uint32 base, uint32 limit, uint8 access, uint8 granularity);
}

#endif
