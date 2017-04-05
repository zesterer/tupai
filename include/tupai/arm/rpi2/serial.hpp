//
// file : serial.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_ARM_RPI2_SERIAL_HPP
#define TUPAI_ARM_RPI2_SERIAL_HPP

// Tupai
#include <tupai/dev/serial.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace arm
	{
		namespace rpi2
		{
			void serial_init();

			size_t       serial_count_ports();
			const char** serial_list_ports();

			bool serial_open_port(int port_id, uint32_t baudrate = 57600, uint8_t databits = 8, uint8_t stopbits = 1, dev::serial_parity parity = dev::serial_parity::NONE);

			void    serial_write(int port_id, uint8_t b);
			uint8_t serial_read(int port_id);
		}
	}
}

#endif
