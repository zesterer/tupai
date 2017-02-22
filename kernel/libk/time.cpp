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

// Libk
#include <libk/time.hpp>
#include <tupai/timer.hpp>

namespace libk
{
	/* Time functions */

	void sleep(useconds_t sec)
	{
		counter_t ctime = tupai::timer_get_nanoseconds();
		counter_t elapsed = sec * 1000 * 1000;

		while (tupai::timer_get_nanoseconds() - ctime < elapsed)
			asm volatile ("int $0x80");
	}

	void usleep(useconds_t usec)
	{
		counter_t ctime = tupai::timer_get_nanoseconds();
		counter_t elapsed = usec * 1000;

		while (tupai::timer_get_nanoseconds() - ctime < elapsed)
			asm volatile ("int $0x80");
	}
}
