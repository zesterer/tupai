/*
* 	file : conv.hpp
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

#ifndef TUPAI_UTIL_CONV_HPP
#define TUPAI_UTIL_CONV_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/util/str.hpp>

namespace tupai
{
	template <typename T>
	safeval<T> parse(const char* str, umem n = ~(umem)0);
	template <typename T, umem N>
	safeval<T> parse(str<char, N> str, umem n = ~(umem)0) { return parse<T>(str.raw(), n); }

	template <typename T>
	safeval<str<char, sizeof(T) * 8 + 1>> compose(T val, int base = 10);
}

#endif
