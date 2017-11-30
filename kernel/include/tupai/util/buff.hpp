//
// file : buff.hpp
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

#ifndef TUPAI_UTIL_BUFF_HPP
#define TUPAI_UTIL_BUFF_HPP

#include <tupai/util/panic.hpp>
#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! Buff<T, N>
	//! Used to provide a safe abstraction around a static memory buffer

	template <typename T, size_t N>
	struct Buff
	{
	private:
		T* _items;

	public:
		Buff(T* buff) : _items(buff) {}

		T& at(size_t i)
		{
			if (i < N)
				return this->_items[i];
			else
				panic("Attempted to access Buff<{}, {}> out of bounds at {}", type_name<T>(), N, i);
		}

		T& operator[](size_t i) { return this->at(i); }

		T& at_unsafe(size_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }
	};

	//! Buff2D<T, W, H>
	//! Used to provide a safe abstraction around a 2-dimensional static memory buffer

	template <typename T, size_t W, size_t H>
	struct Buff2D
	{
	private:
		T* _items;

	public:
		Buff2D(T* buff) : _items(buff) {}

		T& at(size_t x, size_t y)
		{
			if (x < W && y < H)
				return this->_items[y * W + x];
			else
				panic("Attempted to access Buff<{}, {}, {}> out of bounds at {}, {}", type_name<T>(), W, H, x, y);
		}
	};
}

#endif
