//
// file : fmt.hpp
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

#ifndef TUPAI_UTIL_FMT_HPP
#define TUPAI_UTIL_FMT_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/char.hpp>

namespace tupai::util
{
	template <typename S>
	void fmt_arg(S& strm, char c) { strm << c; }

	template <typename S>
	void fmt_arg(S& strm, const char* str)
	{
		while (*str != '\0')
			strm << *(str++);
	}

	template <typename S>
	void _fmt_nth(S& strm, int n) { (void)n; fmt_arg(strm, "N/A"); }
	template <typename S, typename T, typename ... Args>
	void _fmt_nth(S& strm, int n, T item, Args ... args)
	{
		if (n == 0)
			fmt_arg(strm, item);
		else
			_fmt_nth(strm, n - 1, args ...);
	}

	const int _FMT_ARGS_MAX = 4;
	struct _Fmt { int ni = 0; int n[_FMT_ARGS_MAX] = { -1, }; int inc = 0; };
	_Fmt _read_fmt(const char* format);

	template <typename S>
	void fmt(S& strm, const char* format)
	{
		fmt_arg(strm, format);
	}

	template <typename S, typename ... Args>
	constexpr void fmt(S& strm, const char* format, Args ... args)
	{
		bool escaped = false;
		size_t carg = 0;
		for (size_t i = 0; format[i] != '\0'; i ++)
		{
			char c = format[i];
			if (escaped)
			{
				fmt_arg(strm, c);
				escaped = false;
			}
			else
			{
				if (c == '{')
				{
					_Fmt nfmt = _read_fmt(&format[i]);
					if (nfmt.inc > 0)
					{
						i += nfmt.inc;

						if (nfmt.ni >= 1)
							_fmt_nth(strm, nfmt.n[0], args ...);
						else
						{
							_fmt_nth(strm, carg, args ...);
							carg ++;
						}
					}
					else
						fmt_arg(strm, c);
				}
				else if (c == '\\')
					escaped = true;
				else
					fmt_arg(strm, c);
			}
		}
	}
}

#endif
