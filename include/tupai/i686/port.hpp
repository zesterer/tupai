/*
* 	file : port.hpp
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

#ifndef LYRA_I686_PORT_HPP
#define LYRA_I686_PORT_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	namespace i686
	{
		void port_out8(uint16 _port, uint8 _value);
		void port_out16(uint16 _port, uint16 _value);
		void port_out32(uint16 _port, uint32 _value);

		uint8  port_in8(uint16 _port);
		uint16 port_in16(uint16 _port);
		uint32 port_in32(uint16 _port);
	}
}

#endif