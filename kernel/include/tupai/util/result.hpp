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
		template <typename Val>
		class Result
		{
		private:
			bool _okay;
			union {
				Val _val;
				int _err;
			};

		public:
			Result(int err)
			{
				this->_okay = false;
				this->_err = err;
			}

			Result(bool okay, Val val)
			{
				this->_okay = okay;
				this->_val = val;
			}

			bool failed() { return !this->_okay; }
			Val& getValue() { if (!this->_okay) panic("Attempted to realise an erroneous value!"); return this->_val; }
			int getError() { if (this->_okay) panic("No error occured, but code believes it did!"); return this->_err; }
		};

		class Status
		{
		private:
			bool _okay;
			int _err;

		public:
			Status(bool okay, int err)
			{
				this->_okay = okay;
				this->_err = err;
			}

			operator bool() const { return _okay; }

			bool failed() { return !this->_okay; }
			int getError() { if (this->_okay) panic("No error occured, but code believes it did!"); return this->_err; }
		};

		static Status Success() { return Status(true, 0); }
		static Status Failure(int err = 0) { return Status(false, err); }

		template <typename Val> static Result<Val> Success(Val val) { return Result<Val>(true, val); }
		template <typename Val> static Result<Val> Failure(int err = 0) { return Result<Val>(err); }
	}
}

#endif
