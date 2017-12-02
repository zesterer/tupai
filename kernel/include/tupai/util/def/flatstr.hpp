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

#ifndef TUPAI_UTIL_DEF_FLATSTR_HPP
#define TUPAI_UTIL_DEF_FLATSTR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/def/arr.hpp>
#include <tupai/util/traits.hpp>
#include <tupai/util/cstr.hpp>

namespace tupai::util
{
	//! GenFlatStr<T, N>
	//! Provides an abstraction around a fixed-size, stack-allocated string

	template <typename T, size_t N>
	struct GenFlatStr : public IStr<T>
	{
	private:
		Arr<T, N> _str;
		size_t _len;

	public:
		template <size_t X>
		GenFlatStr(const T (&str)[X]) : _str(Arr<T, N>::from(str, X - 1)), _len(X - 1)
		{
			static_assert(X <= N + 1);
		}

		size_t length() { return this->_len; }
		T& at(size_t i);

		template <typename S> typename enable_if<is_stream<S, T>::value>::type print(S& s);
	};

	template <size_t N> using FlatStr = GenFlatStr<char, N>;

	template <typename T, size_t N>
	GenFlatStr<T, N> make_flatstr(const T (&str)[N]) { return GenFlatStr<T, N>(str); }
}

#endif
