//
// file : str.hpp
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

#ifndef TUPAI_UTIL_STR_HPP
#define TUPAI_UTIL_STR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/refstr.hpp>
#include <tupai/util/arr.hpp>
#include <tupai/util/traits.hpp>

namespace tupai::util
{
	//! GenStr<T>
	//! Manages a single-owned heap-allocated string of T items

	template <typename T>
	struct GenStr : public IStr<T>, public IStream<T>
	{
	public:
		typedef T item_type;

	private:
		DynArr<T> _str;

	public:
		template <size_t N> GenStr(const T (&str)[N]) : _str(DynArr<T>::from(str, N)) {}
		GenStr(const T* cstr = "") : _str(DynArr<T>::from(cstr, cstr_len(cstr))) {}

		template <typename A, typename = typename enable_if<is_arr<A, T>::value>::type>
		static GenStr<T> from(A& arr)
		{
			return GenStr<T>(arr.raw_unsafe());
		}

		size_t length() { return this->_str.length(); }
		T& at(size_t i) { return this->_str[i]; }

		GenStr<T>& operator<<(T c);
	};

	//! GenFlatStr<T, N>
	//! Provides an abstraction around a fixed-size, stack-allocated string

	template <typename T, size_t N>
	struct GenFlatStr : public IStr<T>
	{
	private:
		Arr<T, N> _str;

	public:
		GenFlatStr(const T (&str)[N]) : _str(str) {}

		size_t length() { return this->_str.length(); }
		T& at(size_t i) { return this->_str[i]; }
	};

	using Str = GenStr<char>;
	using RefStr = GenRefStr<char>;
	template <size_t N> using FlatStr = GenFlatStr<char, N>;

	template <typename T, size_t N>
	GenFlatStr<T, N> make_flatstr(const T (&str)[N]) { return GenFlatStr<T, N>(str); }
}

// Delayed implementation to prevent circular references
#include <tupai/util/panic.hpp>
#include <tupai/util/typedata.hpp>
#include <tupai/util/arr.hpp>
#include <tupai/util/vec.hpp>

namespace tupai::util
{
	template <typename T>
	GenStr<T>& GenStr<T>::operator<<(T c)
	{
		// TODO : Implement this
		(void)c;
		panic("Tried to insert {0} into GenStr<{0}> without implementation", type_name<T>());
	}
}

#endif
