//
// file : spinlock.cpp
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
