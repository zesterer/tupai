//
// file : fmt.cpp
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

#include <tupai/util/fmt.hpp>

namespace tupai::util
{
	_Fmt _read_fmt(const char* format)
	{
		_Fmt ret;
		int state = 0;
		int ni;
		for (size_t i = 0; format[i] != '\0'; i ++)
		{
			char c = format[i];
			switch (state)
			{
			case 0:
				{
					if (c == '{')
					{
						state = 1;
						ni = 0;
					}
					else
						return ret;
					break;
				}
			case 1:
				{
					if (is_digit(c))
					{
						if (ni == 0)
						{
							ret.ni ++;
							ret.n[ret.ni - 1] = 0;
						}
						else
							ret.n[ret.ni - 1] *= 10;

						ret.n[ret.ni - 1] += c - '0';

						ni ++;
					}
					else if (c == ':' && ret.ni < _FMT_ARGS_MAX - 1)
					{
						ni = 0;
					}
					else if (c == '}')
						return ret;
					else
					{
						ret.inc = 0;
						return ret;
					}
					break;
				}
			}
			ret.inc ++;
		}

		ret.inc = 0;
		return ret;
	}
}
