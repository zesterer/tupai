//
// file : vga.c
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

#include <tupai/arch/x86/vga.h>
#include <tupai/arch/x86/port.h>
#include <tupai/util/log.h>
#include <tupai/def.h>

static const unsigned int COLS = 80;
static const unsigned int ROWS = 25;

static unsigned int col = 0;
static unsigned int row = 0;

static char fg = 0xF;
static char bg = 0x0;

static uint16_t* buffer = (uint16_t*)(VIRT_OFFSET + 0xB8000);

extern uint32_t _vga_col_boot;
extern uint32_t _vga_row_boot;

static uint16_t make_entry(char c, char fg, char bg);

void vga_preinit()
{
	// Import column and row from boot VGA driver
	col = _vga_col_boot;
	row = _vga_row_boot;

	log("[ OK ] VGA driver acquired early VGA parameters\n");
}

void vga_init()
{
	// Nothing yet
}

void vga_putc(char c)
{
	switch (c)
	{
		case '\n':
		{
			col = 0;
			row ++;
		}
		break;

		default:
		{
			unsigned int index = COLS * row + col;
			buffer[index] = make_entry(c, fg, bg);
			col ++;
		}
		break;
	}

	if (col >= COLS)
	{
		col = 0;
		row ++;
	}

	if (row >= ROWS)
	{
		row = ROWS - 1;
		vga_scroll(1);
	}

	vga_place_cursor(col, row);
}

void vga_puts(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++)
		vga_putc(str[i]);
}

void vga_enable_cursor(bool enable)
{
	outb(0x3D4, 0x0A);
	if (enable)
		outb(0x3D5, 0x00);
	else
		outb(0x3D5, 0x3F);
}

void vga_place_cursor(unsigned int col, unsigned int row)
{
	uint16_t pos = COLS * row + col;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_scroll(unsigned int n)
{
	for (unsigned int j = 0; j < ROWS; j ++)
		for (unsigned int i = 0; i < COLS; i ++)
		{
			if (j < ROWS - n)
				buffer[j * COLS + i] = buffer[(j + n) * COLS + i];
			else
				buffer[j * COLS + i] = 0;
		}
}

uint16_t make_entry(char c, char fg, char bg)
{
	return ((uint16_t)((bg << 4) | fg) << 8) | c;
}
