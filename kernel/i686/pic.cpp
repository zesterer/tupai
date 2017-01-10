/*
* 	file : pic.cpp
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
#include <tupai/i686/pic.hpp>
#include <tupai/i686/port.hpp>

namespace tupai
{
	void pic_set_mask(byte irq_line, bool enabled)
	{
		uint16 port;
		uint8  value;

		if (irq_line < 8)
			port = PIC1_DATA;
		else
		{
			port = PIC2_DATA;
			irq_line -= 8;
		}

		if (enabled)
			value = port_in8(port) & ~(1 << irq_line);
		else
			value = port_in8(port) | (1 << irq_line);

		port_out8(port, value);
	}
}
