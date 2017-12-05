//
// file : vec.hpp
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

#ifndef TUPAI_UTIL_DEF_VEC_HPP
#define TUPAI_UTIL_DEF_VEC_HPP

#include <tupai/util/def/dynarr.hpp>
#include <tupai/util/def/box.hpp>
#include <tupai/util/def/cpp.hpp>
#include <tupai/util/traits.hpp>

namespace tupai::util
{
	//! Vec<T>
	//! Manages a heap-allocated LIFO list of T objects

	template <typename T>
	struct Vec
	{
	public:
		typedef T item_type;

		// TODO : Replace internal array accesses with .raw() to prevent double bounds checks & improve performance once we know this 100% works

	private:
		DynArr<_UnsafeBox<T>> _items;
		size_t _len;

		void _resize(size_t ncap)
		{
			DynArr<_UnsafeBox<T>> nitems(ncap);

			for (size_t i = 0; i < this->_len; i ++)
				nitems[i].move_from_unsafe(this->_items[i]);

			this->_items = move(nitems);
		}

	public:
		Vec(size_t reserve = 1);

		template <size_t N>
		Vec(const T (&arr)[N]) : _items(N), _len(N)
		{
			for (size_t i = 0; i < N; i ++)
				this->push(arr[i]);
		}

		~Vec()
		{
			for (size_t i = 0; i < this->_len; i ++)
				this->_items[i].destroy();
		}

		size_t length() { return this->_len; }
		size_t capacity() { return this->_items.length(); }

		T& at(size_t i);

		T& operator[](size_t i) { return this->at(i); }

		void push(T item)
		{
			if (this->length() == this->capacity())
				this->_resize(this->capacity() * 2);

			this->_items[this->_len++].copy_from(item);
		}

		T pop();

		void clear()
		{
			*this = Vec<T>();
		}

		void operator<<(T item)
		{
			this->push(item);
		}

		T operator>>(T& item)
		{
			item = this->pop();
		}

		T& at_unsafe(uintptr_t i) { return *this->_items[i]; }
		T* raw_unsafe() { return &(*this->_items[0]); }

		template <typename S> typename enable_if<is_stream<S>::value>::type print(S& s);
	};
}

#endif
