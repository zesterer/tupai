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

#ifndef TUPAI_UTIL_ARR_HPP
#define TUPAI_UTIL_ARR_HPP

#include <tupai/util/panic.hpp>
#include <tupai/util/assert.hpp>
#include <tupai/util/type.hpp>

// TODO : Added these features later
//#include <initializer_list>

namespace tupai::util
{
	template <typename T, size_t N>
	struct Arr
	{
	private:
		T _items[N];

	public:
		Arr() {}

		// TODO : Added these features later
		// Arr(std::initializer_list<T> args)
		// {
		// 	auto it = args.begin();
		// 	for (size_t i = 0; i < N && it != args.end(); i ++)
		// 		this->_items[i] = *(it++);
		// }

		size_t length() { return N; }

		T& at(size_t i)
		{
			if (i < N)
				return this->_items[i];
			else
				panic("Attempted to access Arr<{}, {}> out of bounds at {}", type_name<T>(), N, i);
		}

		T& operator[](size_t i) { return this->at(i); }

		T& raw(uintptr_t i)
		{
			return this->_items[i];
		}
	};

	template <typename T>
	struct DynArr
	{
	private:
		T* _items;
		size_t _len;

	public:
		DynArr(size_t len) : _items(new T[len]), _len(len) { if (len == 0) panic("Attempted to create DynArr<{}> with zero length", type_name<T>()); }
		~DynArr() { delete[] this->_items; }

		size_t length() { return this->_len; }

		T& at(size_t i)
		{
			if (i < this->_len)
				return this->_items[i];
			else
				panic("Attempted to access DynArr<{}> of length {} out of bounds at {}", type_name<T>(), this->_len, i);
		}

		T& operator[](size_t i) { return this->at(i); }

		T& raw(uintptr_t i)
		{
			return this->_items[i];
		}
	};
}

#endif
