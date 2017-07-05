//
// file : mutex.cpp
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
#include <tupai/util/mutex.hpp>
#include <tupai/util/out.hpp>
#include <tupai/sys/thread.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/interrupt.hpp>

namespace tupai
{
	namespace util
	{
		void mutex_t::lock() volatile
		{
			//sys::call(sys::CALL::LMUTEX, (size_t)this);
		}

		void mutex_t::unlock() volatile
		{
			sys::call(sys::CALL::UMUTEX, (size_t)this);
		}

		/*
		extern "C" void mutex_lock_impl(volatile size_t* val);
		extern "C" void mutex_unlock_impl(volatile size_t* val);

		hw_mutex mutex_hw_mutex;

		bool mutex::is_locked() volatile
		{
			return this->val > 0;
		}

		void mutex::lock() volatile
		{
			if (interrupt_enabled())
			{
				mutex_hw_mutex.lock(); // Begin critical section

				if (this->is_locked())
				{
					sys::thread_wait_signal(sys::thread_get_id(), &this->signal);
					mutex_hw_mutex.unlock(); // End critical section
					mutex_lock_impl(&this->val);
				}
				else
				{
					mutex_lock_impl(&this->val);
					mutex_hw_mutex.unlock(); // End critical section
				}

				this->signal.reset();
			}
		}

		void mutex::unlock() volatile
		{
			//mutex_hw_mutex.lock(); // Begin critical section

			if (interrupt_enabled())
			{
				mutex_unlock_impl(&this->val);
				this->signal.fire();
			}

			//mutex_hw_mutex.unlock(); // End critical section
		}*/

		static volatile bool hw_int_enabled;
		static volatile bool hw_locked = false;
		//static mutex hw_mutex_mutex;

		bool hw_mutex::is_locked() volatile
		{
			return hw_locked;
		}

		void hw_mutex::lock() volatile
		{
			//hw_mutex_mutex.lock(); // Begin critical section

			if (!hw_locked)
			{
				hw_int_enabled = interrupt_enabled();

				if (hw_int_enabled)
					interrupt_enable(false);

				hw_locked = true;
			}

			//hw_mutex_mutex.unlock(); // End critical section
		}

		void hw_mutex::unlock() volatile
		{
			//hw_mutex_mutex.lock(); // Begin critical section

			if (hw_locked)
			{
				if (hw_int_enabled)
					interrupt_enable(true);

				hw_int_enabled = interrupt_enabled();

				hw_locked = false;
			}

			//hw_mutex_mutex.unlock(); // End critical section
		}
	}
}
