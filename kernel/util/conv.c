//
// file : conv.c
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

#include <tupai/util/conv.h>
#include <tupai/util/math.h>

static bool wrapper(int64_t x, int base, int pad, int size, char* str);

bool i8_to_str(int8_t x, int base, int pad, char (str)[I8_STR_MAX])
{
	return wrapper(x, base, pad, sizeof(int8_t), str);
}

bool i16_to_str(int16_t x, int base, int pad, char (str)[I16_STR_MAX])
{
	return wrapper(x, base, pad, sizeof(int16_t), str);
}

bool i32_to_str(int32_t x, int base, int pad, char (str)[I32_STR_MAX])
{
	return wrapper(x, base, pad, sizeof(int32_t), str);
}

bool i64_to_str(int64_t x, int base, int pad, char (str)[I64_STR_MAX])
{
	return wrapper(x, base, pad, sizeof(int64_t), str);
}

bool ptr_to_str(size_t x, int base, int pad, char (str)[PTR_STR_MAX])
{
	int dig = 0;
	for (size_t t = x; t > 0; t /= base) dig ++;

	for (int i = min(sizeof(size_t) * 8, max(1, pad)) - dig; i > 0; i --)
		*(str++) = '0';

	str += dig;
	for (int i = 0; i < dig; i ++)
	{
		switch (base)
		{
			case 2 ... 10:
				*(--str) = '0' + (x % base);
				break;
			case 11 ... 16:
				*(--str) = ((x % base) >= 10) ? ('A' + x % base - 10) : ('0' + x % base);
				break;
			default:
				return false;
		}
		x /= base;
	}

	str += dig;
	*str = '\0';

	return true;
}

bool wrapper(int64_t x, int base, int pad, int size, char* str)
{
	int dig = 0;
	for (int32_t t = abs(x); t > 0; t /= base) dig ++;

	if (x < 0)
	{
		*(str++) = '-';
		x = -x;
	}

	for (int i = min(size * 8, max(1, pad)) - dig; i > 0; i --)
		*(str++) = '0';

	str += dig;
	for (int i = 0; i < dig; i ++)
	{
		switch (base)
		{
			case 2 ... 10:
				*(--str) = '0' + (x % base);
				break;
			case 11 ... 16:
				*(--str) = ((x % base) >= 10) ? ('A' + x % base - 10) : ('0' + x % base);
				break;
			default:
				return false;
		}
		x /= base;
	}

	str += dig;
	*str = '\0';

	return true;
}
