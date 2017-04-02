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

#ifndef TUPAI_DEV_SERIAL_HPP
#define TUPAI_DEV_SERIAL_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace dev
	{
		void serial_init();

		size_t       serial_count_ports();
		const char** serial_list_ports();

		int serial_open_port(const char* port);

		void    serial_write(int port_id, uint8_t val);
		uint8_t serial_read(int port_id);
	}
}

#endif
