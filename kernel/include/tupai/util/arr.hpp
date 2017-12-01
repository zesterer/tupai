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

#include <tupai/util/type.hpp>
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

		static Arr<T, N> from(const T* arr)
		{
			return Arr<T, N>(static_cast<const T (&)[N]>(arr));
		}
	};

	//! DynArr<T>
	//! Provides a safe abstraction around a dynamically-sized, heap-allocated array

	template <typename T>
	struct DynArr : public IArr<T>, public IBuff<T>
	{
	private:
		T* _items;
		size_t _len;

		DynArr(const T* arr, size_t len);

	public:
		DynArr(size_t len);

		template <size_t N>
		DynArr(T (&arr)[N]);

		~DynArr() { delete[] this->_items; }

		size_t length() { return this->_len; }
		T& at(size_t i);

		T& at_unsafe(uintptr_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }

		static DynArr<T> from(const T* arr, size_t len)
		{
			return DynArr<T>(arr, len);
		}
	};

	template <typename T, size_t N>
	Arr<T, N> make_arr(const T (&arr)[N]) { return Arr<T, N>(arr); }

	template <typename T, size_t N>
	DynArr<T> make_dynarr(const T (&arr)[N]) { return DynArr<T>(arr); }
}

// Delayed implementation to prevent circular references
#include <tupai/util/panic.hpp>

namespace tupai::util
{
	template <typename T, size_t N>
	T& Arr<T, N>::at(size_t i)
	{
		if (i < N)
			return this->_items[i];
		else
			panic("Attempted to access Arr<{}, {}> out of bounds at {}", type_name<T>(), N, i);
	}

	template <typename T>
	DynArr<T>::DynArr(const T* arr, size_t len) : _items(new T[len]), _len(len)
	{
		if (this->_len == 0)
			panic("Attempted to create DynArr<{}> with zero length", type_name<T>());

		for (size_t i = 0; i < len; i ++)
			this->_items[i] = arr[i];
	}

	template <typename T>
	DynArr<T>::DynArr(size_t len) : _items(new T[len]), _len(len)
	{
		if (this->_len == 0)
			panic("Attempted to create DynArr<{}> with zero length", type_name<T>());
	}

	template <typename T>
	template <size_t N>
	DynArr<T>::DynArr(T (&arr)[N]) : _items(new T[N]), _len(N)
	{
		if (this->_len == 0)
			panic("Attempted to create DynArr<{}> with zero length", type_name<T>());

		for (size_t i = 0; i < N; i ++)
			this->_items[i] = arr[i];
	}

	template <typename T>
	T& DynArr<T>::at(size_t i)
	{
		if (i < this->_len)
			return this->_items[i];
		else
			panic("Attempted to access DynArr<{}> of length {} out of bounds at {}", type_name<T>(), this->_len, i);
	}
}

#endif
