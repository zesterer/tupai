//
// file : result.hpp
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

#ifndef TUPAI_UTIL_RESULT_HPP
#define TUPAI_UTIL_RESULT_HPP

#include <tupai/util/def/result.hpp>
#include <tupai/util/panic.hpp>
#include <tupai/util/typedata.hpp>

namespace tupai::util
{
	template <typename T, typename E>
	T& Result<T, E>::value()
	{
		if (this->success())
			return this->_either.first();
		else
			panic("Cannot extract value from failed Result<{}>", type_name<E>());
	}

	template <typename T, typename E>
	E& Result<T, E>::error()
	{
		if (this->failed())
			return this->_either.second();
		else
			panic("Cannot extract error from successful Result<{}>", type_name<T>());
	}

	template <typename T, typename E> template <typename ... Args>
	T& Result<T, E>::expect(Args ... args)
	{
		if (this->success())
			return this->_either.first();
		else
			panic(args ...);
	}
}

#endif
