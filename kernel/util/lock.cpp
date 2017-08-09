//
// file : lock.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
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
