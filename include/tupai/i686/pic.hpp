/*
* 	file : pic.hpp
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

#ifndef TUPAI_I686_PIC_HPP
#define TUPAI_I686_PIC_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	const uint16 PIC1      = 0x20;
	const uint16 PIC2      = 0xA0;
	const uint16 PIC1_CMD  = PIC1;
	const uint16 PIC1_DATA = PIC1 + 1;
	const uint16 PIC2_CMD  = PIC2;
	const uint16 PIC2_DATA = PIC2 + 1;

	const uint16 PIC1_START = 0x20; // Or at least it will be after remapping...
	const uint16 PIC2_START = 0x28; // Same as PIC1_START + 1
	const uint16 PIC2_END   = PIC2_START + 8; // Handles 8 (so this is actually the first non-PIC2 interrupt)

	const uint8 PIC_ACK = 0x20;

	void pic_set_mask(byte irq_line, bool enabled);
}

#endif