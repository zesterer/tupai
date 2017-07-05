//
// file : signal.cpp
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
#include <tupai/sys/signal.hpp>
#include <tupai/util/mutex.hpp>

namespace tupai
{
	namespace sys
	{
		static util::hw_mutex hw_mutex;

		void signal_t::reset() volatile
		{
			hw_mutex.lock(); // Begin critical section

			this->fired = false;

			hw_mutex.unlock(); // End critical section
		}

		void signal_t::fire() volatile
		{
			hw_mutex.lock(); // Begin critical section

			this->fired = true;

			hw_mutex.unlock(); // End critical section
		}
	}
}
