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
#include <tupai/util/common.hpp>

namespace tupai::util
{
	// is_arr requirements:
	// - length()
	// - at()
	// - operator[]()
	template <typename T, typename V = typename T::item_type,
		size_t (T::*F0)() = &T::length,
		V& (T::*F1)(size_t) = &T::at
	>
	struct is_arr { static const bool value = is_base_of<T, IArr<V>>::value; };

	// is_buff requirements:
	// - implements is_arr
	// - at_unsafe()
	// - raw_unsafe()
	template <typename T, typename V = typename T::item_type,
		V& (T::*F0)(size_t) = &T::at_unsafe,
		V* (T::*F1)() = &T::raw_unsafe
	>
	struct is_buff { static const bool value = is_base_of<T, IBuff<V>>::value && is_arr<T>::value; };

	// is_arr2d requirements:
	// - width()
	// - height()
	// - at()
	// - operator[]()
	// - at_unsafe()
	// - raw_unsafe()
	template <typename T, typename V = typename T::item_type,
		size_t (T::*F0)() = &T::width,
		size_t (T::*F1)() = &T::height,
		V& (T::*F2)(size_t, size_t) = &T::at
	>
	struct is_arr2d { static const bool value = is_base_of<T, IArr2D<V>>::value; };

	// is_str requirements:
	// - length()
	// - at()
	// - operator[]()
	template <typename T, typename V = typename T::item_type,
		size_t (T::*F0)() = &T::length,
		V& (T::*F1)(size_t) = &T::at
	>
	struct is_str { static const bool value = is_base_of<T, IStr<V>>::value; };

	// is_stream requirements:
	// - operator<<()
	template <typename T, typename V = typename T::item_type,
		T& (T::*F2)(V) = &T::operator<<
	>
	struct is_stream { static const bool value = true; };
}

#endif
