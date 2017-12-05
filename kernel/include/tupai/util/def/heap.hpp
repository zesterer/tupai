//
// file : heap.hpp
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

#ifndef TUPAI_UTIL_DEF_HEAP_HPP
#define TUPAI_UTIL_DEF_HEAP_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! HeapObj<T>
	//! Used to provide a safe abstraction around a heap object

	template <typename T>
	struct HeapObj
	{
	private:
		T* _item;

		HeapObj(T* item) : _item(item) {}

	public:
		template <typename ... Args>
		static HeapObj<T> create(Args ... args);
		void destroy();
	};

	//! HeapArr<T>
	//! Used to provide a safe abstraction around a heap array

	template <typename T>
	struct HeapArr
	{
	private:
		T* _items;
		size_t _len;

		HeapArr(size_t len, T* items) : _items(items), _len(len) {}

	public:
		template <typename ... Args>
		static HeapArr<T> create(size_t len, Args ... args);
		void destroy();

		size_t length() const { return this->_len; }

		T& operator[](size_t i) { return this->_items[i]; }
		const T& operator[](size_t i) const { return this->_items[i]; }
	};
}

#endif
