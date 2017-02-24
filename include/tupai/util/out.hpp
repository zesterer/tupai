/*
* 	file : out.hpp
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

#ifndef TUPAI_UTIL_OUT_HPP
#define TUPAI_UTIL_OUT_HPP

// Tupai
#include <tupai/type.hpp>

// GCC
#include <stdarg.h>

namespace tupai
{
	namespace util
	{
		void printc(char c);
		void print(const char* str);
		void println(const char* str);
		void printf(const char* fmt, ...);// __attribute__((format(printf, 1, 2))); TODO : reinstate this
	}
}

#endif
