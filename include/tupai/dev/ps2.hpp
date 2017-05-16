//
// file : ps2.hpp
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

#ifndef TUPAI_DEV_PS2_HPP
#define TUPAI_DEV_PS2_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace dev
	{
		enum class ps2_device_type
		{
			UNKNOWN,
			KEYBOARD_AT,
			MOUSE_NOSCROLL,
			MOUSE_SCROLL,
			MOUSE_5BUTTON,
			KEYBOARD_MF2,
			KEYBOARD_MF2_TRANSLATION,
		};

		void ps2_init();

		size_t       ps2_count_ports();
		const char** ps2_list_ports();

		int ps2_open_port(const char* port);

		void    ps2_write(int port_id, uint8_t val);
		uint8_t ps2_read(int port_id);

		ps2_device_type ps2_get_device_type(int port_id);
	}
}

#endif
