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

#include <tupai/util/def/fmt.hpp>
#include <tupai/util/char.hpp>
#include <tupai/util/compose.hpp>
#include <tupai/util/result.hpp>

namespace tupai::util
{
	// Character formatting
	template <typename S, typename T>
	typename enable_if<is_stream<S>::value && is_same<T, char>::value>::type
	fmt_arg(S& strm, T c) { strm << c; }

	// String literal formatting
	template <typename S>
	constexpr typename enable_if<is_stream<S>::value>::type
	fmt_arg(S& strm, const char* str)
	{
		while (*str != '\0')
			strm << *(str++);
	}

	// Integer formatting
	template <typename S>
	typename enable_if<is_stream<S>::value>::type
	fmt_arg(S& strm, uint32_t n)
	{
		auto r = compose<char, uint32_t>(n);
		if (r.success())
			fmt_arg(strm, r.value());
	}

	// Pointer formatting
	template <typename S>
	typename enable_if<is_stream<S>::value>::type
	fmt_arg(S& strm, void* ptr)
	{
		(void)strm;
		(void)ptr;
		// Nothing
	}

	// Printable formatting
	template <typename S, typename T>
	typename enable_if<is_stream<S>::value && is_printable<T, S>::value>::type
	fmt_arg(S& strm, T& obj)
	{
		obj.print(strm);
	}

	// Array formatting
	// template <typename S, typename T>
	// typename enable_if<is_stream<S>::value && is_arr<T>::value>::type
	// fmt_arg(S& strm, T& arr)
	// {
	// 	strm << '[';
	// 	for (size_t i = 0; i < arr.length(); i ++)
	// 	{
	// 		if (i > 0)
	// 			fmt_arg(strm, ", ");
	// 		strm << '\'';
	// 		fmt_arg(strm, arr[i]);
	// 		strm << '\'';
	// 	}
	// 	strm << ']';
	// }

	template <typename S>
	constexpr typename enable_if<is_stream<S>::value>::type
	_fmt_nth(S& strm, int n) { (void)n; fmt_arg(strm, "N/A"); }
	template <typename S, typename T, typename ... Args>
	constexpr typename enable_if<is_stream<S>::value>::type
	_fmt_nth(S& strm, int n, T item, Args ... args)
	{
		if (n == 0)
			fmt_arg(strm, item);
		else
			_fmt_nth(strm, n - 1, args ...);
	}

	template <typename S>
	constexpr typename enable_if<is_stream<S>::value>::type
	fmt(S& strm, const char* format)
	{
		fmt_arg(strm, format);
	}

	//! fmt(stream, format, args...)
	//! Formats a string with the specified arguments and inserts them into a stream

	template <typename S, typename ... Args>
	constexpr typename enable_if<is_stream<S>::value>::type
	fmt(S& strm, const char* format, Args ... args)
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
