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
#include <tupai/arm/rpi2/serial.hpp>
#include <tupai/arm/mmio.hpp>
#include <tupai/arm/delay.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace arm
	{
		namespace rpi2
		{
			enum
			{
				PERIPHERAL_BASE = 0x3F000000,

				// The GPIO registers base address.
				GPIO_BASE = PERIPHERAL_BASE + 0x200000,

				// The offsets for reach register.

				// Controls actuation of pull up/down to ALL GPIO pins.
				GPPUD = (GPIO_BASE + 0x94),

				// Controls actuation of pull up/down for specific GPIO pin.
				GPPUDCLK0 = (GPIO_BASE + 0x98),

				// The base address for UART.
				UART0_BASE = GPIO_BASE + 0x1000,

				// The offsets for reach register for the UART.
				UART0_DR     = (UART0_BASE + 0x00),
				UART0_RSRECR = (UART0_BASE + 0x04),
				UART0_FR     = (UART0_BASE + 0x18),
				UART0_ILPR   = (UART0_BASE + 0x20),
				UART0_IBRD   = (UART0_BASE + 0x24),
				UART0_FBRD   = (UART0_BASE + 0x28),
				UART0_LCRH   = (UART0_BASE + 0x2C),
				UART0_CR     = (UART0_BASE + 0x30),
				UART0_IFLS   = (UART0_BASE + 0x34),
				UART0_IMSC   = (UART0_BASE + 0x38),
				UART0_RIS    = (UART0_BASE + 0x3C),
				UART0_MIS    = (UART0_BASE + 0x40),
				UART0_ICR    = (UART0_BASE + 0x44),
				UART0_DMACR  = (UART0_BASE + 0x48),
				UART0_ITCR   = (UART0_BASE + 0x80),
				UART0_ITIP   = (UART0_BASE + 0x84),
				UART0_ITOP   = (UART0_BASE + 0x88),
				UART0_TDR    = (UART0_BASE + 0x8C),
			};

			const char* port_names[1] = {"UART0"};
			const uint8_t  parity_values[5] = { 0b000, 0b010, 0b110 };
			const uint32_t UART_CLOCK_RATE = 3000000;

			void serial_init()
			{
				// Do nothing
			}

			size_t serial_count_ports()
			{
				return 1; // We only have one port, UART0
			}

			const char** serial_list_ports()
			{
				return port_names;
			}

			bool serial_open_port(int port_id, uint32_t baudrate, uint8_t databits, uint8_t stopbits, dev::serial_parity parity)
			{
				if (port_id != 0) // We only have one serial port: UART0
					return false;

				if (databits < 5 || databits > 8)
					return false; // Invalid number of data bits

				if (stopbits < 1 || stopbits > 2)
					return false; // Invalid number of stop bits

				if ((int)parity < 0 || (int)parity > 2)
					return false; // Invalid parity

				// Disable UART0
				mmio_write(UART0_CR, 0x00000000);
				// Setup GPIO pins 14 & 15

				// Disable pull up/down for all GPIO pins
				mmio_write(GPPUD, 0x00000000);
				delay(150);

				// Enable pull up/down for pins 14, 15
				mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
				delay(150);

				// Write 0 to GPPUDCLK0 to make it take effect
				mmio_write(GPPUDCLK0, 0x00000000);

				// Clear pending interrupts
				mmio_write(UART0_ICR, 0x7FF);

				// Set integer & fractional part of serial baud rate
				// Divider = UART_CLOCK / (16 * Baud)
				// Fractional_part_register = (Fractional_part * 64) + 0.5
				// UART_CLOCK = 3000000; Baud = 115200

				// Divider = 3000000 / (16 * baudrate)
				uint32_t dividerx100 = (100 * UART_CLOCK_RATE) / (16 * baudrate);
				uint32_t divider = dividerx100 / 100;
				mmio_write(UART0_IBRD, divider);
				// Fractional_part_register = (frac(divider) * 64) + 0.5
				uint32_t fractional = (((dividerx100 % 100) * 64) + 50) / 100;
				mmio_write(UART0_FBRD, fractional);

				// Enable FIFO & 8-bit data transmission (1 stop bit, no parity)
				uint32_t databits_val = (databits - 5) << 5;
				uint32_t stopbits_val = (stopbits - 1) << 3;
				uint32_t parity_val   = parity_values[(int)parity];
				uint32_t fifo_val     = (1 << 4);
				mmio_write(UART0_LCRH, databits_val | stopbits_val | parity_val | fifo_val);

				// Mask all interrupts
				mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

				// Enable UART0, receive & transfer part of UART
				mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

				return true;
			}

			void serial_write(int port_id, uint8_t b)
			{
				if (port_id != 0) // We only have one serial port: UART0
					return;

				while (mmio_read(UART0_FR) & (1 << 5));
				mmio_write(UART0_DR, b);
			}

			uint8_t serial_read(int port_id)
			{
				if (port_id != 0) // We only have one serial port: UART0
					return 0; // Return null data

				while (mmio_read(UART0_FR) & (1 << 4));
				return mmio_read(UART0_DR);
			}
		}
	}
}
