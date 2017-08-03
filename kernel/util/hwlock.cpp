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
#include <tupai/util/hwlock.hpp>
#include <tupai/irq.hpp>

namespace tupai
{
	namespace util
	{
		static volatile bool hw_int_enabled;
		static volatile bool hw_locked = false;

		bool hwlock_t::is_locked() volatile
		{
			return hw_locked;
		}

		void hwlock_t::lock() volatile
		{
			if (!hw_locked)
			{
				hw_int_enabled = irq::are_enabled();

				if (hw_int_enabled)
					irq::disable();

				hw_locked = true;
			}
		}

		void hwlock_t::unlock() volatile
		{
			if (hw_locked)
			{
				if (hw_int_enabled)
					irq::enable();

				hw_int_enabled = irq::are_enabled();

				hw_locked = false;
			}
		}
	}
}
