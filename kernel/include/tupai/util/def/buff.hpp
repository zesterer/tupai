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

#ifndef TUPAI_UTIL_DEF_BUFF_HPP
#define TUPAI_UTIL_DEF_BUFF_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/common.hpp>

namespace tupai::util
{
	//! Buff<T, N>
	//! Used to provide a safe abstraction around a static memory buffer

	template <typename T, size_t N>
	struct Buff : public IArr<T>, public IBuff<T>
	{
	public:
		typedef T item_type;

	private:
		T* _items;

	public:
		Buff(uintptr_t ptr) : _items(reinterpret_cast<T*>(ptr)) {}

		size_t length() { return N; }
		T& at(size_t i);

		T& at_unsafe(size_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }
	};

	//! Buff2D<T, W, H>
	//! Used to provide a safe abstraction around a 2-dimensional static memory buffer

	template <typename T, size_t W, size_t H>
	struct Buff2D : public IArr2D<T>, public IBuff2D<T>
	{
	private:
		T* _items;

	public:
		Buff2D(uintptr_t ptr) : _items(reinterpret_cast<T*>(ptr)) {}

		size_t width() { return W; }
		size_t height() { return H; }

		T& at(size_t x, size_t y);

		T& at_unsafe(size_t x, size_t y) { return this->_items[y * W + x]; }
		T* raw_unsafe() { return this->_items; }
	};

	//! DynBuff<T>
	//! Used to provide a safe abstraction around a dynamically-sized memory buffer

	template <typename T>
	struct DynBuff : public IArr<T>, public IBuff<T>
	{
	public:
		typedef T item_type;

	private:
		T* _items;
		size_t _len;

	public:
		DynBuff(uintptr_t ptr, size_t len) : _items(reinterpret_cast<T*>(ptr)), _len(len) {}

		size_t length() { return this->_len; }
		T& at(size_t i);

		T& at_unsafe(size_t i) { return this->_items[i]; }
		T* raw_unsafe() { return this->_items; }
	};
}

#endif
