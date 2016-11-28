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
#include <tupai/kpanic.hpp>

#include <tupai/i686/kbd.hpp>

namespace tupai
{
	bool cmp_str(const char* str0, const char* str1)
	{
		for (umem i = 0; str0[i] != '\0' && str1[i] != '\0'; i ++)
		{
			if (str0[i] != str1[i])
				return false;
		}
		return true;
	}

	int prompt()
	{
		//tty_clear();
		tty_write_str("\n>");

		const umem INPUT_BUFFER_LENGTH = 1024;
		char buffer[INPUT_BUFFER_LENGTH];
		umem buffer_pos = 0;
		while (true)
		{
			char input_char = i686::key_char;
			i686::key_char = '\0';

			if (input_char != '\0')
			{
				if (input_char == '\n' || buffer_pos == INPUT_BUFFER_LENGTH - 1)
				{
					buffer[buffer_pos] = '\0';
					break;
				}
				else
				{
					tty_write(input_char);

					buffer[buffer_pos] = input_char;
					buffer_pos ++;
				}
			}
		}

		tty_write('\n');

		if (cmp_str(buffer, "help"))
		{
			tty_write_str("--- Help ---\n");
			tty_write_str("sys   - Show system information\n");
			tty_write_str("panic - Perform a kernel panic\n");
		}
		else if (cmp_str(buffer, "sys"))
		{
			tty_write_str("Tupai OS 0.1.0 on ");
			tty_write_str(SYSTEM_ARCH);
			tty_write('\n');
		}
		else if (cmp_str(buffer, "panic"))
		{
			kpanic();
		}

		return 0;
	}
}
