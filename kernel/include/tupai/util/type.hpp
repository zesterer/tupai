//
// file : type.hpp
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

#ifndef TUPAI_UTIL_TYPE_HPP
#define TUPAI_UTIL_TYPE_HPP

#include <stdint.h>
#include <stddef.h>

namespace tupai::util
{
	//! type_name()
	//! Emits a static string containing a human-readable description of the type T

	template <typename T>
	constexpr const char* type_name()
	{
		return __PRETTY_FUNCTION__ + 57; // TODO : This is a total hack. Maintain it well, and replace it when the C++ standard matures
	}

	template <typename T, typename U> struct is_same { static const bool value = false; };
	template <typename T> struct is_same<T, T> { static const bool value = true; };

	template<bool cond, class T = void> struct enable_if {};
	template <typename T> struct enable_if<true, T> { typedef T type; };

	struct true_type { static const bool value = true; };
	struct false_type { static const bool value = false; };

	template <typename T> true_type is_base_of_func(T*);
	template <typename T> false_type is_base_of_func(void*);
	template <typename T, typename U> struct is_base_of { static const bool value = decltype(is_base_of_func<U>(static_cast<T*>(nullptr)))::value; };
}

#endif
