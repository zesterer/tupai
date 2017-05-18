//
// file : debug.cpp
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
#include <tupai/debug.hpp>
#include <tupai/dev/tty.hpp>

namespace tupai
{
	bool debug_initiated = false;
	int  debug_serial_port_id;

	void debug_init()
	{
		#if defined(DEBUG_ENABLED)
		{
			if (debug_initiated)
				return;

			debug_initiated = true;
		}
		#endif
	}

	void debug_write(char c __attribute__((unused)))
	{
		#if defined(DEBUG_ENABLED)
			dev::tty_write(c);
		#endif
	}

	void debug_print(const char* str __attribute__((unused)))
	{
		#if defined(DEBUG_ENABLED)
		{
			dev::tty_print("[DEBUG] "); // Debug prefix
			dev::tty_print(str);
		}
		#endif
	}
}
