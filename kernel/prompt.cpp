/*
* 	file : prompt.cpp
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
#include <tupai/prompt.hpp>
#include <tupai/tty.hpp>

#include <tupai/i686/kbd.hpp>

namespace tupai
{
	int prompt()
	{
		//tty_clear();
		tty_write_str(">");

		volatile unsigned long i = 0;
		while (true)
		{
			i = (i + 1) % 40000000;

			if (i == 0) tty_write('@');
			//if (i686::key_char != '\0')
			//	tty_write(i686::key_char % 128);
		}

		while (true) asm volatile ("hlt");
	}
}
