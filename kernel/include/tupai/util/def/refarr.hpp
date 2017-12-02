//
// file : refarr.hpp
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

#ifndef TUPAI_UTIL_DEF_REFARR_HPP
#define TUPAI_UTIL_DEF_REFARR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/traits.hpp>

namespace tupai::util
{
	//! RefArr<T>
	//! Provides a safe reference to an array

	template <typename T>
	struct RefArr
	{
	public:
		typedef T item_type;

	private:
		T* _items;
		size_t _len;

	public:
		RefArr(T* arr, size_t len) : _items(arr), _len(len) {}

		size_t length() { return this->_len; }
		T& at(size_t i);

		T& operator[](size_t i) { return this->at(i); }

		T& at_unsafe(uintptr_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }

		template <typename S> typename enable_if<is_stream<S>::value>::type print(S& s);
	};
}

#endif
