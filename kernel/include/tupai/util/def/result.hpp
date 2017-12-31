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

#ifndef TUPAI_UTIL_DEF_RESULT_HPP
#define TUPAI_UTIL_DEF_RESULT_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/mem.hpp>

namespace tupai::util
{
	template <typename T, typename U>
	struct Either
	{
		static_assert(!is_same<T, U>::value, "Types within 'Either' cannot be identical");

		bool _is_first;
		union
		{
			T _first;
			U _second;
		};

		Either(Either<T, U>&& o)
		{
			mem_copy(reinterpret_cast<uint8_t*>(&o), reinterpret_cast<uint8_t*>(this), sizeof(Either<T, U>));
		}

		Either(T val) : _is_first(true), _first(val) {}
		Either(U val) : _is_first(false), _second(val) {}

		bool is_first() { return this->_is_first; }

		T& first() { return this->_first; }
		U& second() { return this->_second; }
	};

	//! Result<T, U>
	//! Provides optional return values (monadic function results)

	template <typename T, typename E>
	struct Result
	{
	private:
		Either<T, E> _either;

	public:
		Result(T val) : _either(val) {}
		Result(E err) : _either(err) {}

		bool success() { return this->_either.is_first(); }
		bool failed() { return !this->success(); }

		T& value();
		E& error();

		template <typename ... Args> T& expect(Args ... args);
	};
}

#endif
