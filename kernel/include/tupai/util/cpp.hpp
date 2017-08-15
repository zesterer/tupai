//
// file : cpp.hpp
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

#ifndef TUPAI_UTIL_CPP_HPP
#define TUPAI_UTIL_CPP_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		/*
		template <typename T>
		T&& move(T&& arg)
		{
			return static_cast<T&&>(arg);
		}
		*/

		template< class T > struct remove_reference      {typedef T type;};
		template< class T > struct remove_reference<T&>  {typedef T type;};
		template< class T > struct remove_reference<T&&> {typedef T type;};

		template <typename T>
		typename remove_reference<T>::type&& move(T&& arg) noexcept { return static_cast<typename remove_reference<decltype(arg)>::type&&>(arg); }
	}
}

#endif
