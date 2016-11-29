/*
* 	file : idt.hpp
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

#ifndef TUPAI_I686_IDT_HPP
#define TUPAI_I686_IDT_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	struct idt_entry
	{
		uint16 base_addr_low;
		uint16 selector;
		uint8  zero;
		uint8  type_attr;
		uint16 base_addr_high;

	} __attribute((packed));

	struct idt_desc
	{
		uint32 size;
		uint32 offset;

	} __attribute((packed));

	const umem IDT_REMAP_OFFSET = 0x20;
	const umem IDT_SIZE = 256 + IDT_REMAP_OFFSET;

	void idt_init();
	void idt_install();
	void idt_set_entry(smem irq, uint32 address, uint16 selector = 0x08);
}

#endif
