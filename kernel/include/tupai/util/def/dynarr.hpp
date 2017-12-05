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

#ifndef TUPAI_UTIL_DEF_DYNARR_HPP
#define TUPAI_UTIL_DEF_DYNARR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/traits.hpp>
#include <tupai/util/def/heap.hpp>

namespace tupai::util
{
	//! DynArr<T>
	//! Provides a safe abstraction around a dynamically-sized, heap-allocated array

	template <typename T>
	struct DynArr
	{
	public:
		typedef T item_type;

	private:
		HeapArr<T> _items;

		//DynArr(const T* arr, size_t len);

		// DynArr<T>& _copy(const DynArr<T>& other)
		// {
		// 	this->_items.create(other.length());
		// 	this->_len = other.length();
        //
		// 	for (size_t i = 0; i < this->_len; i ++)
		// 		this->_items[i] = other._items[i];
        //
		// 	return *this;
		// }

	public:
		DynArr(size_t len);
		DynArr(DynArr<T>&& other) : _items(other._items) {}

		~DynArr() { this->_items.destroy(); }

		DynArr(const DynArr<T>& other) : _items(HeapArr<T>::create(other.length()))
		{
			for (size_t i = 0; i < this->length(); i ++)
				this->_items[i] = other._items[i];
		}

		DynArr<T>& operator=(const DynArr<T>& other)
		{
			this->_items.destroy();

			this->_items = HeapArr<T>::create(other.length());
			for (size_t i = 0; i < this->length(); i ++)
				this->_items[i] = other._items[i];
			return *this;
		}

		template <size_t N>
		DynArr(T (&arr)[N]);

		size_t length() const { return this->_items.length(); }
		T& at(size_t i);

		T& operator[](size_t i) { return this->at(i); }

		T& at_unsafe(uintptr_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }

		template <typename S> typename enable_if<is_stream<S>::value>::type print(S& s);

		static DynArr<T> from(const T* arr, size_t len)
		{
			return DynArr<T>(arr, len);
		}
	};
}

#endif
