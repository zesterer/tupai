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
	}
}
