//
// file : common.hpp
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

#ifndef TUPAI_UTIL_COMMON_HPP
#define TUPAI_UTIL_COMMON_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	template <typename T>
	struct IArr
	{
	public:
		typedef T item_type;

	public:
		virtual size_t length();
		virtual T& at(size_t i);
		T& operator[](size_t i) { return this->at(i); }
	};

	template <typename T>
	struct IArr2D
	{
	public:
		typedef T item_type;

	public:
		virtual size_t width();
		virtual size_t height();
		virtual T& at(size_t x, size_t y);
	};

	template <typename T>
	struct IBuff
	{
	public:
		virtual T& at_unsafe(size_t i);
		virtual T* raw_unsafe();
	};

	template <typename T>
	struct IBuff2D
	{
	public:
		virtual T& at_unsafe(size_t x, size_t y);
		virtual T* raw_unsafe();
	};
}

#endif
