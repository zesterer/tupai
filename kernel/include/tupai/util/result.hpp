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

// Tupai
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename Val, typename Err = int>
		class result
		{
		private:
			bool _is_err;
			union {
				Val _val;
				Err _err;
			};

		public:
			result()
			{
				this->_is_err = true;
			}

			result(Val val)
			{
				this->_is_err = false;
				this->_val = val;
			}

			/*
			result(Err err)
			{
				this->_is_err = true;
				this->_err = err;
			}
			*/

			bool failed() { return this->_is_err; }
			Val& get_value() { if (this->_is_err) panic("Attempted to realise an erroneous value!"); return this->_val; }
			Err& get_error() { if (!this->_is_err) panic("No error occured, but code believes it did!"); return this->_err; }
		};
	}
}

#endif
