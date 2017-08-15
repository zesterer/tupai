//
// file : conv.cpp
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

// Tupai
#include <tupai/util/conv.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/math.hpp>

namespace tupai
{
	namespace util
	{
		template<typename T>
		bool compose_integer(T val, char* buff, size_t n, int base, int pad)
		{
			// Make sure our buffer is large enouh
			if (n < sizeof(T) * 8 + 4)
				return false;

			if (base <= 0)
				return false;

			// 'Full' padding
			if (pad == -1)
			{
				switch (base)
				{
				case 2:
					pad = sizeof(T) * 8;
					break;
				case 16:
					pad = sizeof(T) * 2;
					break;
				}
			}

			// Find size
			int digits = 0;
			for (T test = abs(val); test > 0; test /= base) { digits ++; }

			size_t str_count = 0;

			// Sign
			if (val < 0)
			{
				buff[str_count] = '-';
				str_count ++;

				val = abs(val);
			}

			// Base
			switch (base)
			{
			case 2:
				{
					buff[str_count] = '0';
					buff[str_count + 1] = 'b';
					str_count += 2;
					break;
				}
			case 8:
				{
					buff[str_count] = '0';
					buff[str_count + 1] = 'o';
					str_count += 2;
					break;
				}
			case 16:
				{
					buff[str_count] = '0';
					buff[str_count + 1] = 'x';
					str_count += 2;
					break;
				}
			default:
				break;
			}

			// Add padding (inc val = 0 case)
			pad = min((int)(sizeof(T) * 8), max(1, pad));
			for (int i = 0; i < (pad - (int)digits); i ++)
			{
				buff[str_count] = '0';
				str_count ++;
			}

			switch (base)
			{
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				{
					str_count += digits;
					for (int i = 0; i < digits; i ++)
					{
						buff[str_count - i - 1] = '0' + (val % base);
						val /= base;
					}
				}
				break;

			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
				{
					str_count += digits;
					for (int i = 0; i < digits; i ++)
					{
						buff[str_count - i - 1] = ((val % base) >= 10) ? ('A' + val % base - 10) : ('0' + val % base);
						val /= base;
					}
				}
				break;

			default:
				return false;
			};
			buff[str_count] = '\0';

			return true;
		}

		bool compose(int8_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }
		bool compose(uint8_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }

		bool compose(int16_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }
		bool compose(uint16_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }

		bool compose(int32_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }
		bool compose(uint32_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }

		bool compose(int64_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }
		bool compose(uint64_t val, char* buff, size_t n, int base, int pad) { return compose_integer(val, buff, n, base, pad); }

		bool compose(void* val, char* buff, size_t n)
		{
			return compose_integer((size_t)val, buff, n, 16, sizeof(void*) * 2);
		}

		bool compose(bool val, char* buff, size_t n)
		{
			if (n < 6)
				return false;

			str_cpy(val ? "true" : "false", buff);
			return true;
		}
	}
}
