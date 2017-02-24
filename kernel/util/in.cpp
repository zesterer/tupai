/*
* 	file : in.cpp
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
#include <tupai/util/in.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/kbd.hpp>
#endif

namespace tupai
{
	namespace util
	{
		bool is_input()
		{
			#if defined(SYSTEM_ARCH_i686)
				return kbd_ringbuffer.length() > 0;
			#else
				true;
			#endif
		}

		int getc()
		{
			#if defined(SYSTEM_ARCH_i686)
				while (true)
				{
					if (kbd_ringbuffer.length() > 0)
						return kbd_ringbuffer.pop();
					else
						asm volatile ("int $0x80");
				}
			#else
				return '\0';
			#endif
		}
	}
}
