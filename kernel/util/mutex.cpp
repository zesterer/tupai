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
#include <tupai/interrupt.hpp>

namespace tupai
{
	namespace util
	{
		extern "C" void mutex_lock_impl(volatile size_t* val);
		extern "C" void mutex_unlock_impl(volatile size_t* val);

		bool mutex::is_locked() volatile
		{
			return this->val > 0;
		}

		void mutex::lock() volatile
		{
			mutex_lock_impl(&this->val);
		}

		void mutex::unlock() volatile
		{
			mutex_unlock_impl(&this->val);
		}

		bool hw_mutex::is_locked() volatile
		{
			return this->locked;
		}

		void hw_mutex::lock() volatile
		{
			if (this->locked)
				return;

			this->int_enabled = interrupt_enabled();

			if (this->int_enabled)
				interrupt_enable(false);
		}

		void hw_mutex::unlock() volatile
		{
			if (!this->locked)
				return;

			if (this->int_enabled)
				interrupt_enable(true);

			this->int_enabled = interrupt_enabled();
		}
	}
}
