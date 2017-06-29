//
// file : serial.cpp
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

// Tupai
#include <tupai/x86/serial.hpp>
#include <tupai/x86/port.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		static const char*    port_names[4] = {"COM1", "COM2", "COM3", "COM4"};
		static const uint16_t port_offsets[4] = { 0x03F8, 0x02F8, 0x03E8, 0x02E8 };
		static const uint8_t  parity_values[5] = { 0b000000, 0b001000, 0b011000, 0b101000, 0b111000 };
		static const uint32_t UART_CLOCK_RATE = 115200;

		static bool serial_initiated = false;
		static bool serial_port_open[4] = { false, };

		void serial_init()
		{
			serial_initiated = true;
		}

		size_t serial_count_ports()
		{
			return 4;
		}

		const char** serial_list_ports()
		{
			return port_names;
		}

		bool serial_open_port(int port_id, uint32_t baudrate, uint8_t databits, uint8_t stopbits, dev::serial_parity parity)
		{
			if (port_id == -1) // Make sure the port id is valid
				return false;

			if (serial_port_open[port_id])
				return false; // It's already open

			// Find the serial port's I/O port
			uint16_t port_offset = port_offsets[port_id];

			if (databits < 5 || databits > 8)
				return false; // Invalid number of data bits

			if (stopbits < 1 || stopbits > 2)
				return false; // Invalid number of stop bits

			outb(port_offset + 1, 0x00); // Disable serial interrupts

			// Setting the divisor
			uint16_t divisor = UART_CLOCK_RATE / baudrate;
			outb(port_offset + 3, 0x80); // Enable DLAB (to set baud rate divisor)
			outb(port_offset + 0, (divisor >> 0) & 0xFF); // Lo byte
			outb(port_offset + 1, (divisor >> 8) & 0xFF); // Hi byte

			// Calculate serial configuration
			uint8_t databits_val = databits - 5;
			uint8_t stopbits_val = (stopbits - 1) << 2;
			uint8_t parity_val = parity_values[(int)parity];
			uint8_t serial_cfg = databits_val | stopbits_val | parity_val;
			outb(port_offset + 3, serial_cfg); // Set serial configuration
			outb(port_offset + 2, 0xC7); // Enable FIFO and clear (with a 14-byte buffer)
			outb(port_offset + 4, 0x0B); // Reenable serial interrupts

			// Flag the port as open
			serial_port_open[port_id] = true;

			return true;
		}

		void serial_write(int port_id, uint8_t c)
		{
			if (port_id == -1) // Make sure the port id is valid
				return;

			uint16_t port_offset = port_offsets[port_id]; // Find the serial port's I/O port

			while ((inb(port_offset + 5) & 0x20) == 0); // Wait until port is ready for writing
			outb(port_offset, c);
		}

		uint8_t serial_read(int port_id)
		{
			if (port_id == -1) // Make sure the port id is valid
				return 0; // Return null data

			uint16_t port_offset = port_offsets[port_id]; // Find the serial port's I/O port

			while ((inb(port_offset + 5) & 0x1) == 0); // Wait until port is ready for reading
			return inb(port_offset);
		}
	}
}
