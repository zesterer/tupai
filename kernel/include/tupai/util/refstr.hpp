//
// file : refstr.hpp
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

#ifndef TUPAI_UTIL_REFSTR_HPP
#define TUPAI_UTIL_REFSTR_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/arr.hpp>
#include <tupai/util/traits.hpp>

namespace tupai::util
{
	template <typename T>
	inline size_t cstr_len(const T* str)
	{
		size_t i = 0;
		for (; str[i] != '\0'; i ++);
		return i;
	}

	//! GenStrRef<T>
	//! References a constant (within the lifetime of the reference) string of T items

	template <typename T>
	struct GenRefStr : public IStr<const T>
	{
	private:
		const T* _str;
		size_t _len;

	public:
		template <size_t N> GenRefStr(const T (&str)[N]) : _str(str), _len(N) {}
		GenRefStr(const T* str) : _str(str), _len(cstr_len(str)) {}
		GenRefStr(const T* str, size_t len) : _str(str), _len(len) {}

		template <typename A, typename = typename enable_if<is_arr<A, T>::value>::type>
		static GenRefStr<T> from(A& arr)
		{
			return GenRefStr<T>(arr.raw_unsafe(), arr.length());
		}

		size_t length() { return this->_len; }
		const T& at(size_t i);
	};

	using RefStr = GenRefStr<char>;
}

// Delayed implementation to prevent circular references
#include <tupai/util/panic.hpp>
#include <tupai/util/typedata.hpp>

namespace tupai::util
{
	template <typename T>
	const T& GenRefStr<T>::at(size_t i)
	{
		if (i < this->_len)
			return this->_str[i];
		else
			panic("Attempted to access StrRef<{}> of length {} out of bounds at {}", type_name<T>(), this->_len, i);
	}
}

#endif
