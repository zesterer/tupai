//
// file : arr.hpp
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

#ifndef TUPAI_UTIL_DEF_ARR_HPP
#define TUPAI_UTIL_DEF_ARR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/attr.hpp>
#include <tupai/util/common.hpp>

namespace tupai::util
{
	//! Arr<T, N>
	//! Provides a safe abstraction around a fixed-size, stack-allocated array

	template <typename T, size_t N>
	struct Arr : public IArr<T>, public IBuff<T>
	{
	private:
		T _items[N];

		Arr(const T* arr, size_t len) : _items()
		{
			for (size_t i = 0; i < len; i ++)
				this->_items[i] = arr[i];
		}

	public:
		Arr() {}

		Arr(const T (&arr)[N])
		{
			for (size_t i = 0; i < N; i ++)
				this->_items[i] = arr[i];
		}

		size_t length() { return N; }
		T& at(size_t i);

		T& at_unsafe(size_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }

		static Arr<T, N> from(const T* arr, size_t len)
		{
			return Arr<T, N>(static_cast<const T*>(arr), len);
		}

		template <typename S> typename enable_if<is_stream<S>::value>::type print(S& s);
	} ATTR_PACKED;

	template <typename T, size_t N>
	Arr<T, N> make_arr(const T (&arr)[N]) { return Arr<T, N>(arr); }
}

#endif
