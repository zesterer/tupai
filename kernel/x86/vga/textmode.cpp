//
// file : log.cpp
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

#include <tupai/x86/vga/textmode.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/util/buff.hpp>
#include <tupai/util/box.hpp>
#include <tupai/util/boot.hpp>
#include <tupai/util/attr.hpp>
#include <tupai/mem/virt.hpp>

namespace tupai::x86::vga::textmode
{
	struct Entry
	{
		uint8_t data;
		uint8_t color;

		Entry(uint8_t data, uint8_t color) : data(data), color(color) {}
		Entry(uint8_t data, uint8_t fg, uint8_t bg) : data(data), color((bg << 4) | fg) {}

		static Entry blank() { return Entry(' ', 0xF, 0x0); }
	} ATTR_PACKED;

	const size_t COLS = 80;
	const size_t ROWS = 25;

	size_t col = 0, row = 0;
	uint8_t fg = 0xF, bg = 0x0;

	static util::Box<util::Buff2D<Entry, COLS, ROWS>> buffer;

	extern "C" uint32_t _vga_col_boot;
	extern "C" uint32_t _vga_row_boot;

	void init()
	{
		col = _vga_col_boot;
		row = _vga_row_boot;

		buffer.create(reinterpret_cast<Entry*>(mem::virt::OFFSET + 0xB8000));

		enable_cursor();

		util::bootlog("Acquired early VGA parameters");
	}

	void write_char(char c)
	{
		switch (c)
		{
		case '\n': // Newline
			col = 0;
			row ++;
			break;

		case '\0': // Ignore '\0'
			break;

		default:
			buffer->at(col, row) = Entry(c, fg, bg);
			col ++;
			break;
		}

		if (col >= COLS) {
			col = 0;
			row ++;
		}

		if (row >= ROWS) {
			row = ROWS - 1;
			scroll(1);
		}

		move_cursor(col, row);
	}

	void move_cursor(size_t col, size_t row)
	{
		uint16_t off = COLS * row + col;
		io::out<8>(0x3D4, 0x0F);
		io::out<8>(0x3D5, (uint8_t)(off & 0xFF));
		io::out<8>(0x3D4, 0x0E);
		io::out<8>(0x3D5, (uint8_t)((off >> 8) & 0xFF));
	}

	void enable_cursor()
	{
		io::out<8>(0x3D4, 0x0A);
		io::out<8>(0x3D5, 0x00);
	}

	void disable_cursor()
	{
		io::out<8>(0x3D4, 0x0A);
		io::out<8>(0x3D5, 0x3F);
	}

	void scroll(size_t n)
	{
		for (size_t j = 0; j < ROWS; j ++)
			for (size_t i = 0; i < COLS; i ++)
			{
				if (j < ROWS - n)
					buffer->at(i, j) = buffer->at(i, j + n);
				else
					buffer->at(i, j) = Entry::blank();
			}
	}
}
