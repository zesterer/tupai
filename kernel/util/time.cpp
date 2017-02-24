/*
* 	file : time.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/util/time.hpp>

#include <tupai/sys/timer.hpp>

namespace tupai
{
	namespace util
	{
		void sleep(uint64 sec)
		{
			counter_t ctime = sys::timer_get_nanoseconds();
			counter_t elapsed = sec * 1000 * 1000;

			while (sys::timer_get_nanoseconds() - ctime < elapsed)
				asm volatile ("int $0x80");
		}

		void usleep(uint64 usec)
		{
			counter_t ctime = sys::timer_get_nanoseconds();
			counter_t elapsed = usec * 1000;

			while (sys::timer_get_nanoseconds() - ctime < elapsed)
				asm volatile ("int $0x80");
		}
	}
}
