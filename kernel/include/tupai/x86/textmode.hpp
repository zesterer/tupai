//
// file : textmode.hpp
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

#ifndef TUPAI_X86_TEXTMODE_HPP
#define TUPAI_X86_TEXTMODE_HPP

// Tupai
#include <tupai/util/ansi.hpp>

namespace tupai
{
	namespace x86
	{
		void textmode_init();

		void textmode_cursor_enable();
		void textmode_cursor_disable();
		void textmode_cursor_move(int col, int row);
		void textmode_move(int ncol, int nrow);

		void textmode_write(char c);

		void textmode_scroll(int n);
		void textmode_clear();

		void textmode_set_fg(uint8_t color);
		void textmode_set_bg(uint8_t color);

		void textmode_apply(util::ansi_cmd_t cmd);
		void textmode_apply_sgr(int n);
	}
}

#endif
