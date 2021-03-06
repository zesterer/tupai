//
// file : status.hpp
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

#ifndef TUPAI_UTIL_DEF_STATUS_HPP
#define TUPAI_UTIL_DEF_STATUS_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! Status<T>
	//! Provides optional error value

	template <typename T>
	struct Status
	{
	private:
		bool _success;
		union { T _err; };

	public:
		Status() : _success(true) {}
		Status(T err) : _success(false), _err(err) {}

		bool success() { return this->_success; }
		bool failed() { return !this->success(); }

		template <typename ... Args>
		void expect(Args ... args);

		T error();
	};

}

#endif
