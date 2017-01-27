/*
* 	file : serial.hpp
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

#ifndef TUPAI_I686_SERIAL_HPP
#define TUPAI_I686_SERIAL_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	void serial_init();
	void serial_open(uint16 port, int32 baudrate = 57600, uint8 databits = 8, uint8 stopbits = 1, uint8 parity = 0x8);
	void serial_write(uint16 port, char c);
	void serial_write_str(uint16 port, const char* str);
}

#endif
