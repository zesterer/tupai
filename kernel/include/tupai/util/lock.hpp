//
// file : lock.hpp
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

#ifndef TUPAI_UTIL_LOCK_HPP
#define TUPAI_UTIL_LOCK_HPP

// Tupai
#include <tupai/task/task.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		class lock_t;

		class key_t
		{
			friend class lock_t;
		private:
			volatile lock_t* _lock;

			key_t(volatile lock_t* lock) { this->_lock = lock; }

		public:
			void release() volatile;
			~key_t() { this->release(); }
		};

		class lock_t
		{
			friend class key_t;
		private:
			volatile size_t           _ref = 0;
			volatile task::thrd_ptr_t _owner = ID_INVALID;
			volatile size_t           _val = 0;

			void lock() volatile;
			void unlock() volatile;
			void force_unref() volatile;

		public:
			key_t acquire() volatile;
		};
	}
}

#endif
