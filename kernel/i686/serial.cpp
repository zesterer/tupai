/*
* 	file : serial.cpp
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

// Tupai
#include <tupai/i686/serial.hpp>
#include <tupai/i686/port.hpp>

namespace tupai
{
	const static uint32 UART_CLOCK_RATE = 115200;

	bool port_open[4] = { false, false, false, false };

	static uint16 serial_get_port_value(uint16 port);
	static bool serial_can_write(uint16 port);

	void serial_init()
	{
		// Do nothing for now
	}

	void serial_open(uint16 port, int32 baudrate, uint8 databits, uint8 stopbits, uint8 parity)
	{
		uint16 port_value;
		uint8  databits_value;
		uint8  stopbits_value;

		// Find port value
		if (port < 1 || port > 4) // Invalid port
			return;
		else
			port_value = serial_get_port_value(port);

		// Find databits value
		if (databits < 5 || databits > 8) // Invalid port
			return;
		else
		{
			const uint8 databits_values[] = { 0x0, 0x1, 0x2, 0x3 };
			databits_value = databits_values[databits - 5];
		}

		// Find stopbits value
		if (stopbits < 1 || stopbits > 2) // Invalid port
			return;
		else
		{
			const uint8 stopbits_values[] = { 0x4, 0x0 };
			stopbits_value = stopbits_values[databits - 5];
		}

		uint16 divisor = (uint16)(UART_CLOCK_RATE / baudrate);

		port_out8(port_value + 1, 0x00); // Disable interrupts
		port_out8(port_value + 3, 0x80); // Enable DLAB (set baudrate divisor)

		// Divisor upper and lower bits
		port_out8(port_value + 0, (divisor >> 0) & 0xFF); // Low byte
		port_out8(port_value + 1, (divisor >> 8) & 0xFF); // High byte

		byte serial_config = databits_value | stopbits_value | (byte)parity;
		port_out8(port_value + 3, serial_config); // Write serial configuration
		port_out8(port_value + 2, 0xC7); // Enable FIFO and clear (14-byte threshold)
		port_out8(port_value + 4, 0x0B); // Reenable IRQ

		port_open[port - 1] = true;
	}

	void serial_write(uint16 port, char c)
	{
		// Is it even open?
		if (!port_open[port - 1])
			return;

		// Find port value
		uint16 port_value;
		if (port < 1 || port > 4) // Invalid port
			return;
		else
			port_value = serial_get_port_value(port);

		while (!serial_can_write(port));

		port_out8(port_value, c);
	}

	void serial_write_str(uint16 port, const char* str)
	{
		for (umem i = 0; str[i] != '\0'; i ++)
			serial_write(port, str[i]);
	}

	static uint16 serial_get_port_value(uint16 port)
	{
		const uint16 port_values[] = { 0x3F8, 0x2F8, 0x3E8, 0x2E8 };
		return port_values[port - 1];
	}

	static bool serial_can_write(uint16 port)
	{
		byte val = port_in8(port + 5);
		return (val & 0x20) == 0;
	}
}
