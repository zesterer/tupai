//
// file : char.hpp
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

#ifndef TUPAI_UTIL_CHAR_HPP
#define TUPAI_UTIL_CHAR_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		bool is_newline(char c);

		bool is_whitespace(char c)
		{
			return c == ' ' || c == '\n' || is_newline(c);
		}

		bool is_printable(char c)
		{
			return ((unsigned char)c >= 0x20) && ((unsigned char)c <= 0x7F);
		}

		bool is_newline(char c)
		{
			return c == '\n' || c == '\r';
		}

		bool is_backspace(char c)
		{
			return c == '\b';
		}

		bool is_escape(char c)
		{
			return c == 0x1B;
		}

		bool is_digit(char c)
		{
			return c >= '0' && c <= '9';
		}

		bool is_hex(char c)
		{
			return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
		}

		bool is_lowercase(char c)
		{
			return c >= 'a' && c <= 'z';
		}

		bool is_uppercase(char c)
		{
			return c >= 'A' && c <= 'Z';
		}

		char hex_to_num(char c)
		{
			return ((c >= '0' && c <= '9') ? (c - '0') :
				((c >= 'a' && c <= 'f') ? (c - 'a' + 10) :
				((c >= 'A' && c <= 'F') ? (c - 'A' + 10) : 0xFF)));
		}

		char digit_to_num(char c)
		{
			return ((c >= '0' && c <= '9') ? (c - '0') : 0xFF);
		}

		char num_to_hex(char n)
		{
			return ((n >= 0 && n <= 9) ? ('0' + n) : ((n >= 0xA && n <= 0xF) ? ('A' + n - 10) : '!'));
		}

		char num_to_digit(char n)
		{
			return ((n >= 0 && n <= 9) ? ('0' + n) : '!');
		}

		char to_lowercase(char c)
		{
			return is_uppercase(c) ? (c - 'A' + 'a') : c;
		}

		char to_uppercase(char c)
		{
			return is_lowercase(c) ? (c - 'a' + 'A') : c;
		}
	}
}

#endif
