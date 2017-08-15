//
// file : spinlock.cpp
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

// Tupai
#include <tupai/util/spinlock.hpp>

namespace tupai
{
	namespace util
	{
		extern "C" void spinlock_lock_impl(volatile size_t* val);
		extern "C" void spinlock_unlock_impl(volatile size_t* val);

		bool spinlock_t::is_locked() volatile
		{
			this->lock(); // Begin critical section

			bool locked = this->val > 0;

			this->lock(); // End critical section

			return locked;
		}

		void spinlock_t::lock() volatile
		{
			spinlock_lock_impl(&this->val);
		}

		void spinlock_t::unlock() volatile
		{
			spinlock_unlock_impl(&this->val);
		}
	}
}
