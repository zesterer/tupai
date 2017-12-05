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

#ifndef TUPAI_UTIL_DEF_FMT_HPP
#define TUPAI_UTIL_DEF_FMT_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/traits.hpp>

namespace tupai::util
{
	// Character formatting
	template <typename S, typename T>
	typename enable_if<is_stream<S>::value && is_same<T, char>::value>::type
	fmt_arg(S& strm, T c);

	// String literal formatting
	template <typename S>
	constexpr typename enable_if<is_stream<S>::value>::type
	fmt_arg(S& strm, const char* str);

	// Integer formatting
	template <typename S>
	typename enable_if<is_stream<S>::value>::type
	fmt_arg(S& strm, uint32_t n);

	// Pointer formatting
	template <typename S>
	typename enable_if<is_stream<S>::value>::type
	fmt_arg(S& strm, void* ptr);

	// Array formatting
	// template <typename S, typename T>
	// typename enable_if<is_stream<S>::value && is_arr<T>::value>::type
	// fmt_arg(S& strm, T& arr);

	template <typename S>
	constexpr typename enable_if<is_stream<S>::value>::type
	_fmt_nth(S& strm, int n);
	template <typename S, typename T, typename ... Args>
	constexpr typename enable_if<is_stream<S>::value>::type
	_fmt_nth(S& strm, int n, T item, Args ... args);

	const int _FMT_ARGS_MAX = 4;
	struct _Fmt { int ni = 0; int n[_FMT_ARGS_MAX] = { -1, }; int inc = 0; };
	_Fmt _read_fmt(const char* format);

	template <typename S>
	constexpr typename enable_if<is_stream<S>::value>::type
	fmt(S& strm, const char* format);

	//! fmt(stream, format, args...)
	//! Formats a string with the specified arguments and inserts them into a stream

	template <typename S, typename ... Args>
	constexpr typename enable_if<is_stream<S>::value>::type
	fmt(S& strm, const char* format, Args ... args);
}

#endif
