//
// file : dynarr.hpp
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

#ifndef TUPAI_UTIL_DYNARR_HPP
#define TUPAI_UTIL_DYNARR_HPP

#include <tupai/util/def/dynarr.hpp>
#include <tupai/util/panic.hpp>
#include <tupai/util/typedata.hpp>

namespace tupai::util
{
	// template <typename T>
	// DynArr<T>::DynArr(const T* arr, size_t len) : _items(new T[len]), _len(len)
	// {
	// 	if (this->_len == 0)
	// 		panic("Attempted to create DynArr<{}> with zero length", type_name<T>());
    //
	// 	for (size_t i = 0; i < len; i ++)
	// 		this->_items[i] = arr[i];
	// }

	template <typename T>
	DynArr<T>::DynArr(size_t len) : _items(Dyn<T>::create_arr(len))
	{
		if (this->length() == 0)
			panic("Attempted to create DynArr<{}> with zero length", type_name<T>());
	}

	template <typename T>
	template <size_t N>
	DynArr<T>::DynArr(T (&arr)[N]) : _items(Dyn<T>::create_arr(N))
	{
		if (this->length() == 0)
			panic("Attempted to create DynArr<{}> with zero length", type_name<T>());

		for (size_t i = 0; i < this->length(); i ++)
			this->_items[i] = arr[i];
	}

	template <typename T>
	T& DynArr<T>::at(size_t i)
	{
		if (i < this->length())
			return this->_items[i];
		else
			panic("Attempted to access DynArr<{}> of length {} out of bounds at {}", type_name<T>(), this->length(), i);
	}

	template <typename T>
	template <typename S>
	typename enable_if<is_stream<S>::value>::type DynArr<T>::print(S& s)
	{
		fmt_arg(s, '[');
		for (size_t i = 0; i < this->length(); i ++)
		{
			if (i > 0)
				fmt_arg(s, ", ");
			fmt_arg(s, this->at(i));
		}
		fmt_arg(s, ']');
	}
}

#endif
