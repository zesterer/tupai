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
#include <tupai/dev/serial.hpp>

namespace tupai
{
	bool debug_initiated = false;
	int debug_serial_port_id;

	void debug_init()
	{
		if (debug_initiated)
			return;

		dev::serial_init();

		// Find the names of available serial ports
		const char** serial_port_names = dev::serial_list_ports();
		// Search the serial port list, trying to open a debugging port
		for (size_t i = 0; i < dev::serial_count_ports() && debug_serial_port_id == -1; i ++)
			debug_serial_port_id = dev::serial_open_port(serial_port_names[i]);

		if (debug_serial_port_id != -1)
		{
			debug_print("[DBG] Started serial debug output on ");
			debug_print(serial_port_names[debug_serial_port_id]);
			debug_print(".\n");
		}
		else
			debug_print("[DBG] Could not find port for serial debug output!\n");

		debug_initiated = true;
	}

	void debug_write(char c)
	{
		dev::serial_write(debug_serial_port_id, c);
		if (c == '\n') // Serial debugging interfaces regard a carriage return as a newline
			dev::serial_write(debug_serial_port_id, '\r');
	}

	void debug_print(const char* str)
	{
		for (size_t i = 0; str[i] != '\0'; i ++)
			debug_write(str[i]);
	}
}
