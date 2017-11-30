//
// file : traits.hpp
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

#ifndef TUPAI_UTIL_TRAITS_HPP
#define TUPAI_UTIL_TRAITS_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	// is_arr requirements:
	// - length()
	// - at()
	// - operator[]()
	// - at_unsafe()
	// - raw_unsafe()
	template <typename T, typename V,
		size_t (T::*F0)() = &T::length,
		V& (T::*F1)(size_t) = &T::at,
		V& (T::*F2)(size_t) = &T::operator[],
		V& (T::*F3)(size_t) = &T::at_unsafe,
		V* (T::*F4)() = &T::raw_unsafe
	>
	struct is_arr { static const bool value = true; };

	// is_str requirements:
	// - length()
	// - at()
	// - operator[]()
	template <typename T, typename V,
		size_t (T::*F0)() = &T::length,
		V (T::*F1)(size_t) = &T::at,
		V (T::*F2)(size_t) = &T::operator[]
	>
	struct is_str { static const bool value = true; };

	// is_stream requirements:
	// - operator<<()
	template <typename T,
		void (T::*F2)(char) = &T::operator<<
	>
	struct is_stream { static const bool value = true; };
}

#endif
