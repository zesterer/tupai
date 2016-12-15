/*
* 	file : timer.cpp
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
#include <tupai/prog/timer.hpp>

// Libk
#include <libk/stdio.hpp>
#include <libk/time.hpp>

using namespace libk;

namespace tupai
{
	namespace prog
	{
		int timer_main(int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused)))
		{
			printf("Counting 5 seconds in second increments...\n");

			for (int i = 0; i < 5; i ++)
			{
				libk::sleep(1);
				printf("Second %i.\n", i + 1);
			}

			printf("Counting 1 second in 50 millisecond increments...\n");

			for (int i = 0; i < 20; i ++)
			{
				libk::usleep(50);
				printf("Millisecond %i.\n", (i + 1) * 50);
			}

			printf("Done!\n");

			return 0;
		}
	}
}
