//
// file : ps2_8042.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_X86_PS2_8042_HPP
#define TUPAI_X86_PS2_8042_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		void ps2_8042_init();

		size_t       ps2_8042_count_ports();
		const char** ps2_8042_list_ports();

		bool ps2_8042_open_port(int port_id);

		void    ps2_8042_write(int port_id, uint8_t c);
		uint8_t ps2_8042_read(int port_id);
	}
}

#endif
