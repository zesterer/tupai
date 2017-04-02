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

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/serial.hpp>
#elif defined(ARCH_rpi2)
	#include <tupai/arm/rpi2/serial.hpp>
#else
	#warning "Architecture provides no debugging interface!"
#endif

namespace tupai
{
	bool debug_initiated = false;

	void debug_init()
	{
		if (debug_initiated)
			return;

		#if defined(ARCH_FAMILY_x86)
		{
			x86::serial_init();
			x86::serial_open(x86::serial_port::COM1);
			debug_print("Started kernel debugging on COM1\n");
		}
		#elif defined(ARCH_rpi2)
		{
			arm::rpi2::serial_init();
			debug_print("Started kernel debugging on UART0\n");
		}
		#endif

		debug_initiated = true;
	}

	void debug_write(char c)
	{
		#if defined(ARCH_FAMILY_x86)
		{
			x86::serial_write(x86::serial_port::COM1, c);
			if (c == '\n') // Serial debugging interfaces regard a carriage return as a newline
				x86::serial_write(x86::serial_port::COM1, '\r');
		}
		#elif defined(ARCH_rpi2)
		{
			arm::rpi2::serial_write(c);
			if (c == '\n') // Serial debugging interfaces regard a carriage return as a newline
				arm::rpi2::serial_write('\r');
		}
		#endif
	}

	void debug_print(const char* str)
	{
		// Display debugging prefix
		const char* prefix = "DEBUG: ";
		for (size_t i = 0; prefix[i] != '\0'; i ++)
			debug_write(prefix[i]);

		for (size_t i = 0; str[i] != '\0'; i ++)
			debug_write(str[i]);
	}
}
