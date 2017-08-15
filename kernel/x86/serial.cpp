//
// file : serial.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

// Tupai
#include <tupai/x86/serial.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/spinlock.hpp>

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

		static util::spinlock_t spinlock;

		void serial_init()
		{
			spinlock.lock(); // Begin critical section

			serial_initiated = true;

			spinlock.unlock(); // End critical section
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
			spinlock.lock(); // Begin critical section

			if (port_id != -1 && !serial_port_open[port_id]) // Make sure the port id is valid and that it isn't already open
			{
				// Find the serial port's I/O port
				uint16_t port_offset = port_offsets[port_id];

				if (databits < 5 || databits > 8)
				{
					spinlock.unlock(); // End critical section
					return false; // Invalid number of data bits
				}

				if (stopbits < 1 || stopbits > 2)
				{
					spinlock.unlock(); // End critical section
					return false; // Invalid number of stop bits
				}

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
			}

			spinlock.unlock(); // End critical section

			return true;
		}

		void serial_write(int port_id, uint8_t c)
		{
			if (port_id == -1) // Make sure the port id is valid
				return;

			spinlock.lock(); // Start critical section

			uint16_t port_offset = port_offsets[port_id]; // Find the serial port's I/O port

			if ((inb(port_offset + 5) & 0x20) == 0) // Wait if port is not ready for writing
				wait(150);
			outb(port_offset, c);

			spinlock.unlock(); // End critical section
		}

		uint8_t serial_read(int port_id)
		{
			if (port_id == -1) // Make sure the port id is valid
				return 0; // Return null data

			spinlock.lock(); // Start critical section

			uint16_t port_offset = port_offsets[port_id]; // Find the serial port's I/O port

			uint8_t val = 0;
			if ((inb(port_offset + 5) & 0x1) == 0) // Wait until port is ready for reading
				val = inb(port_offset);

			spinlock.unlock(); // End critical section

			return val;
		}
	}
}
