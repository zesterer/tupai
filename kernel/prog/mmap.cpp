/*
* 	file : mmap.cpp
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
#include <tupai/util/conv.hpp>

#include <tupai/util/string.hpp>

#include <tupai/fs/common.hpp>

#include <tupai/memory.hpp>

namespace tupai
{
	namespace prog
	{
		int mmap_main(int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused)))
		{
			util::print("--- Memory Map ---\n");

			//util::string ps = "this/is/a/test";
			fs::path p("this/is/a/test");
			util::printf("%s, %i\n", p.str().raw(), p.length());

			umem mem_size_kb = memory_get_size_kb();
			umem mem_frame_count = (mem_size_kb / MEMORY_FRAME_SIZE) * 1024;

			uint32 volatile pid = ~(uint32)0;
			byte flags = 0xFF;
			umem first = 0;

			for (umem i = 0; i < mem_frame_count + 1; i ++)
			{
				umem address = i * MEMORY_FRAME_SIZE;
				const memory_phys_frame* frame = memory_get_frame(address);

				if (frame->pid != pid || frame->flags != flags || i == mem_frame_count)
				{
					if (i != 0)
					{
						util::print("0x");
						util::print(util::compose(first, 16, 8).val().raw());
						util::print("-0x");
						util::print(util::compose(address - 1, 16, 8).val().raw());
						util::print(" : ");

						if (flags & MEMORY_PHYS_FLAG_USED)
						{
							util::print("pid = ");
							util::print(util::compose(pid, 10).val().raw());
							util::print(", flags = 0b");
							util::print(util::compose(flags, 2, 8).val().raw());
						}
						else
						{
							util::print("UNUSED");
						}

						util::printc('\n');
					}

					pid = frame->pid;
					flags = frame->flags;

					first = address;
				}
			}

			return 0;
		}
	}
}
