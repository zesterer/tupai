//
// file : dynstr.hpp
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

#ifndef TUPAI_UTIL_DEF_DYNSTR_HPP
#define TUPAI_UTIL_DEF_DYNSTR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/def/dynarr.hpp>
#include <tupai/util/traits.hpp>
#include <tupai/util/cstr.hpp>

namespace tupai::util
{
	//! GenDynStr<T>
	//! Manages a single-owned heap-allocated string of T items

	template <typename T>
	struct GenDynStr : public IStr<T>, public IStream<T>
	{
	public:
		typedef T item_type;

	private:
		DynArr<T> _str;

	public:
		template <size_t N> GenDynStr(const T (&str)[N]) : _str(DynArr<T>::from(str, N)) {}
		GenDynStr(const T* cstr = "") : _str(DynArr<T>::from(cstr, cstr_len(cstr))) {}

		template <typename A, typename = typename enable_if<is_arr<A, T>::value>::type>
		static GenDynStr<T> from(A& arr)
		{
			return GenDynStr<T>(arr.raw_unsafe());
		}

		size_t length() { return this->_str.length(); }
		T& at(size_t i) { return this->_str[i]; }

		template <typename S> typename enable_if<is_stream<S, T>::value>::type print(S& s);

		GenDynStr<T>& operator<<(T c);
	};

	using DynStr = GenDynStr<char>;
}

#endif
