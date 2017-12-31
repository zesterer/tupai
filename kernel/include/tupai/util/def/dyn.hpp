//
// file : dyn.hpp
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

#ifndef TUPAI_UTIL_DEF_DYN_HPP
#define TUPAI_UTIL_DEF_DYN_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! Dyn<T, N>
	//! Used to provide a safe abstraction around (a) heap object(s)

	template <typename T>
	struct Dyn
	{
	private:
		T* _items = nullptr;
		size_t _len = 0;

		Dyn(T* items, size_t len) : _items(items), _len(len) {}

		Dyn<T>& copy(const Dyn<T>& other);
		Dyn<T>& move(Dyn<T>& other)
		{
			this->_items = other._items;
			this->_len = other._len;

			// Invalidate old
			other._items = nullptr;
			other._len = 0;

			return *this;
		}

	public:
		Dyn(const Dyn<T>& other) // Copy construction
		{
			this->copy(other);
		}

		Dyn<T>& operator=(const Dyn<T>& other) // Copy assignment
		{
			return this->copy(other);
		}

		Dyn(Dyn<T>&& other) // Move construction
		{
			this->move(other);
		}

		Dyn<T>& operator=(Dyn<T>&& other) // Move assignment
		{
			return this->move(other);
		}

		~Dyn();

		template <typename ... Args> static Dyn<T> create(Args ... args);
		template <typename ... Args> static Dyn<T> create_arr(size_t len, Args ... args);

		size_t length() const { return this->_len; }

		T& item();
		T& item(size_t i);

		T& operator[](size_t i) { return this->item(i); }
	};
}

#endif
