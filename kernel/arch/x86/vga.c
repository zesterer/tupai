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

#include <tupai/def.h>

static const int COLS = 80;
static const int ROWS = 25;

static int col = 0;
static int row = 0;

static char fg = 0xF;
static char bg = 0x0;

static uint16_t* buffer = (uint16_t*)(VIRT_OFFSET + 0xB8000);

extern uint32_t _vga_col_boot;
extern uint32_t _vga_row_boot;

static uint16_t make_entry(char c, char fg, char bg);

void vga_init()
{
	// Import column and row from boot VGA driver
	col = _vga_col_boot;
	row = _vga_row_boot;
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
			int index = COLS * row + col;
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
		row = 0;
		col = 0;
	}
}

void vga_puts(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++)
		vga_putc(str[i]);
}

uint16_t make_entry(char c, char fg, char bg)
{
	return ((uint16_t)((bg << 4) | fg) << 8) | c;
}
