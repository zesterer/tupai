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

#include <tupai/util/conv.hpp>

#include <tupai/tty.hpp>
#include <tupai/memory.hpp>

namespace tupai
{
	namespace prog
	{
		int mmap_main(int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused)))
		{
			tty_write_str("--- Memory Map ---\n");

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
						tty_write_str("0x");
						tty_write_str(util::compose(first, 16).val().raw());
						tty_write_str("-0x");
						tty_write_str(util::compose(address - 1, 16).val().raw());
						tty_write_str(" : pid = ");
						tty_write_str(util::compose(pid, 10).val().raw());
						tty_write_str(", flags = 0b");
						tty_write_str(util::compose(flags, 2).val().raw());
						tty_write_str("\n");
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
