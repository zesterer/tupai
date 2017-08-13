//
// file : conv.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_UTIL_CONV_HPP
#define TUPAI_UTIL_CONV_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		bool compose(int8_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint8_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(int16_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint16_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(int32_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint32_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(int64_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint64_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(void* val, char* buff, size_t n);
		bool compose(bool val, char* buff, size_t n);
	}
}

#endif
