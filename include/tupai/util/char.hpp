//
// file : char.hpp
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

#ifndef TUPAI_UTIL_CHAR_HPP
#define TUPAI_UTIL_CHAR_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
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
				((c >= 'A' && c <= 'F') ? (c - 'A' + 10) : (0, false))));
		}

		char digit_to_num(char c)
		{
			return ((c >= '0' && c <= '9') ? (c - '0') : (0, false));
		}

		char num_to_hex(char n)
		{
			return ((n >= 0 && n <= 9) ? ('0' + n) : ((n >= 0xA && n <= 0xF) ? ('A' + n - 10) : ('0', false)));
		}

		char num_to_digit(char n)
		{
			return ((n >= 0 && n <= 9) ? ('0' + n) : ('0', false));
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
