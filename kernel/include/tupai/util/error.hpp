//
// file : error.hpp
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

#ifndef TUPAI_UTIL_ERROR_HPP
#define TUPAI_UTIL_ERROR_HPP

// Tupai
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	static const bool ERROR_NONE = false;

	template <typename Err>
	class error
	{
	private:
		bool _is_err;
		Err _err;

	public:
		error(bool is_err)
		{
			this->_is_err = is_err;
		}

		error(Err err)
		{
			this->_is_err = true;
			this->_err = err;
		}

		bool failed() { return this->_is_err; }
		Err& get_error() { if (!this->_is_err) panic("No error occured, but code believes it did!"); return this->_err; }
	};
}

#endif
