//
// file : ps2_8042.cpp
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

// Tupai
#include <tupai/x86/ps2_8042.hpp>

namespace tupai
{
	namespace x86
	{
		static const char* port_names[4] = {"PS20", "PS21"};

		void ps2_8042_init()
		{
			// Do nothing
		}

		size_t ps2_8042_count_ports()
		{
			return 2;
		}

		const char** ps2_8042_list_ports()
		{
			return port_names;
		}

		bool ps2_8042_open_port(int port_id);

		void    ps2_8042_write(int port_id, uint8_t c);
		uint8_t ps2_8042_read(int port_id);
	}
}
