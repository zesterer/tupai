//
// file : uart.c
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
#include <tupai/arm/uart.h>
#include <tupai/arm/mmio.h>
#include <tupai/arm/delay.h>

// Standard
#include <stddef.h>
#include <stdint.h>

enum
{
	// The GPIO registers base address.
	GPIO_BASE = 0x20200000, // 0x3F200000 for raspi2 and raspi3

	// The offsets for reach register.

	// Controls actuation of pull up/down to ALL GPIO pins.
	GPPUD = (GPIO_BASE + 0x94),

	// Controls actuation of pull up/down for specific GPIO pin.
	GPPUDCLK0 = (GPIO_BASE + 0x98),

	// The base address for UART.
	UART0_BASE = 0x20201000, // 0x3F201000 for raspi 2 and raspi3

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

void uart_init()
{
	// Disable UART0
	mmio_write(UART0_CR, 0x00000000);
	// Setup GPIO pins 14 & 15

	// Disable pull up/down for all GPIO pins
	mmio_write(GPPUD, 0x00000000);
	delay(150);

	// Disable pull up/down for pins 14, 15
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

	// Divider = 3000000 / (16 * 115200) = 1.627 = ~1
	mmio_write(UART0_IBRD, 1);
	// Fractional_part_register = (0.627 * 64) + 0.5 = 40.6 = ~40
	mmio_write(UART0_FBRD, 40);

	// Enable FIFO & 8-bit data transmission (1 stop bit, no parity)
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	// Mask all interrupts
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	// Enable UART0, receive & transfer part of UART
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

	//while (1)
	//	uart_write(uart_read());
}

void uart_write(uint8_t b)
{
	while (mmio_read(UART0_FR) & (1 << 5));
	mmio_write(UART0_DR, b);
}

uint8_t uart_read()
{
	while (mmio_read(UART0_FR) & (1 << 4));
	return mmio_read(UART0_DR);
}
