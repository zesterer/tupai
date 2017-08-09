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
#include <tupai/cpu.hpp>

namespace tupai
{
	namespace util
	{
		static volatile int hwlock_ref = 0;
		static volatile bool irq_enabled = false;

		void hwlock_acquire()
		{
			if (cpu::is_irq())
				return;

			bool irq = irq::are_enabled();
			irq::disable();

			if (hwlock_ref == 0)
				irq_enabled = irq;
			else if (irq_enabled)
				hwlock_ref ++;
		}

		void hwlock_release()
		{
			if (cpu::is_irq())
				return;

			if (irq_enabled)
			{
				hwlock_ref --;
				if (hwlock_ref == 0)
					irq::enable();
			}
		}
	}
}
