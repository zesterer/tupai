//
// file : tty.cpp
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
#include <tupai/tty.hpp>
#include <tupai/dev/serial.hpp>
#include <tupai/debug.hpp>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/textmode.hpp>
#endif

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	bool tty_initiated = false;
	int tty_serial_port = -1;

	void tty_init()
	{
		if (tty_initiated)
			return;

		x86::textmode_init();

		// Find the names of available serial ports
		const char** serial_port_names = dev::serial_list_ports();
		// Search the serial port list, trying to open a debugging port
		for (size_t i = 0; i < dev::serial_count_ports() && tty_serial_port == -1; i ++)
			tty_serial_port = dev::serial_open_port(serial_port_names[i], 57600, 8, 1, dev::serial_parity::NONE);

		if (tty_serial_port != -1)
		{
			debug_print(
				"Started serial tty output on ", serial_port_names[tty_serial_port], '\n',
				"  baudrate -> ", 57600, '\n',
				"  databits -> ", 8, '\n',
				"  stopbits -> ", 1, '\n',
				"  parity   -> ", "NONE", '\n'
			);
		}
		else
			debug_print("Could not find port for serial tty output!\n");

		tty_initiated = true;
	}

	void tty_write(char c)
	{
		x86::textmode_write(c);

		dev::serial_write(tty_serial_port, c);
		if (c == '\n') // Serial interfaces regard a carriage return as a newline
			dev::serial_write(tty_serial_port, '\r');
	}

	void tty_print(const char* str)
	{
		for (size_t i = 0; str[i] != '\0'; i ++)
			tty_write(str[i]);
	}

	char tty_read()
	{
		return dev::serial_read(tty_serial_port);
	}

	void tty_readline(char* buff, size_t n)
	{
		size_t i = 0;
		while (i + 1 < n)
		{
			char c = tty_read();
			tty_write(c);

			if (c == '\r')
				break;

			if (c != '\0')
			{
				buff[i] = c;
				i ++;
			}
		}
		buff[i] = '\0';
	}
}
