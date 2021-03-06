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

#ifndef TUPAI_UTIL_VEC_HPP
#define TUPAI_UTIL_VEC_HPP

#include <tupai/util/def/vec.hpp>
#include <tupai/util/dynarr.hpp>
#include <tupai/util/box.hpp>
#include <tupai/util/dyn.hpp>
#include <tupai/util/cpp.hpp>
#include <tupai/util/panic.hpp>

namespace tupai::util
{
	template<typename T>
	Vec<T>::Vec(size_t reserve) : _items(reserve), _len(0)
	{
		if (reserve == 0)
			panic("Cannot initiate Vec<{}> with a reserved size of zero", type_name<T>());
	}

	template<typename T>
	T& Vec<T>::at(size_t i)
	{
		if (i < this->_len)
			return *this->_items[i];
		else
			panic("Attempted to access Vec<{}> of length {} out of bounds at {}", type_name<T>(), this->_len, i);
	}

	template <typename T>
	T Vec<T>::pop()
	{
		if (this->_len == 0)
			panic("Attempted to pop item from empty Vec<{}>", type_name<T>());

		this->_items[--this->_len].destroy();

		if (this->length() < this->capacity() / 2 && this->length() > 1)
			this->_resize(this->capacity() * 2);
	}

	template <typename T>
	template <typename S>
	typename enable_if<is_stream<S>::value>::type Vec<T>::print(S& s)
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
