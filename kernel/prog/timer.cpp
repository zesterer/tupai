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

#include <tupai/util/out.hpp>
#include <tupai/util/time.hpp>
#include <tupai/util/conv.hpp>
#include <tupai/util/cstr.hpp>

namespace tupai
{
	namespace prog
	{
		int timer_main(int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused)))
		{
			int mode = 0; // 0 = Seconds, 1 = Milliseconds
			bool chosen_time = false;
			uint64 time = 0;

			for (int i = 0; i < argc; i ++)
			{
				if (util::cstr_equal(argv[i], "-s"))
					mode = 0;
				else if (util::cstr_equal(argv[i], "-u"))
					mode = 1;
				else
				{
					safeval<uint64> n = util::parse<uint64>(argv[i]);

					if (n.is_valid())
					{
						if (!chosen_time)
						{
							time = n.val();
							chosen_time = true;
						}
						else
						{
							util::println("Error: multiple time values given");
							return 1;
						}
					}
					else
					{
						util::printf("Error: '%s' is not a valid integer\n", argv[i]);
						return 1;
					}
				}
			}

			if (!chosen_time)
			{
				util::println("Error: time not specified");
				return 1;
			}

			util::printf("Counting %u %s...\n", (uint32)time, (mode == 0) ? "seconds" : "milliseconds");
			for (uint64 i = 0; i < time; i ++)
			{
				if (mode == 0)
					util::sleep(1);
				else
					util::usleep(1);

				util::printf("%u\n", (uint32)i + 1);
			}

			util::println("Done!");

			return 0;
		}
	}
}
