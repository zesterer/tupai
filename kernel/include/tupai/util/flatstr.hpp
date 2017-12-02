//
// file : flatstr.hpp
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

#ifndef TUPAI_UTIL_FLATSTR_HPP
#define TUPAI_UTIL_FLATSTR_HPP

#include <tupai/util/def/flatstr.hpp>
#include <tupai/util/fmt.hpp>
#include <tupai/util/panic.hpp>
#include <tupai/util/typedata.hpp>
#include <tupai/util/arr.hpp>

namespace tupai::util
{
	template <typename T, size_t N>
	T& GenFlatStr<T, N>::at(size_t i)
	{
		if (i < N)
			return this->_str[i];
		else
			panic("Attempted to access StrRef<{}> of length {} out of bounds at {}", type_name<T>(), this->_len, i);
	}

	template <typename T, size_t N>
	template <typename S>
	typename enable_if<is_stream<S, T>::value>::type GenFlatStr<T, N>::print(S& s)
	{
		util::fmt(s, this->_str.raw_unsafe());
	}
}

#endif
