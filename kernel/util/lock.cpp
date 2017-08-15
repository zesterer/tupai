//
// file : lock.cpp
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
#include <tupai/util/lock.hpp>
#include <tupai/util/hwlock.hpp>
#include <tupai/task/task.hpp>

namespace tupai
{
	namespace util
	{
		extern "C" void spinlock_lock_impl(volatile size_t* val);
		extern "C" void spinlock_unlock_impl(volatile size_t* val);

		void key_t::release() volatile
		{
			if (this->_lock == nullptr)
				return;

			hwlock_acquire(); // Begin critical section

			this->_lock->force_unref();
			this->_lock = nullptr;

			util::hwlock_release(); // End critical section
		}

		key_t lock_t::acquire() volatile
		{
			hwlock_acquire(); // Begin critical section

			if (this->_owner.id == task::get_current_thread())
			{
				this->_ref ++;

				util::hwlock_release(); // End critical section
				return key_t(this);
			}
			else
			{
				util::hwlock_release(); // End critical section

				this->lock();
				this->_owner.id = task::get_current_thread();
				this->_ref ++;
				return key_t(this);
			}
		}

		void lock_t::lock() volatile
		{
			spinlock_lock_impl(&this->_val);
		}

		void lock_t::unlock() volatile
		{
			spinlock_unlock_impl(&this->_val);
		}

		void lock_t::force_unref() volatile
		{
			hwlock_acquire(); // Begin critical section

			if (this->_owner.id != task::get_current_thread())
			{
				util::hwlock_release(); // End critical section
				return;
			}

			this->_ref --;

			if (this->_ref == 0)
			{
				this->_owner.id = ID_INVALID;
				this->unlock();
			}

			util::hwlock_release(); // End critical section
		}
	}
}
