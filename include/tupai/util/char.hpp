/*
* 	file : char.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_UTIL_CHAR_HPP
#define TUPAI_UTIL_CHAR_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	namespace util
	{
		bool is_printable(char c);
		bool is_newline(char c);
		bool is_backspace(char c);
		bool is_escape(char c);
		bool is_digit(char c);
		bool is_hex(char c);

		bool is_lowercase(char c);
		bool is_uppercase(char c);

		char hex_to_num(char c);
		char digit_to_num(char c);

		char num_to_hex(char c);
		char num_to_digit(char c);
	}
}

#endif
