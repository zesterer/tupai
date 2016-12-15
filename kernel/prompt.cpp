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

#include <tupai/prog/snake.hpp>
#include <tupai/prog/adventure.hpp>
#include <tupai/prog/sys.hpp>
#include <tupai/prog/timer.hpp>

// Libk
#include <libk/stdlib.hpp>
#include <libk/stdio.hpp>
#include <libk/string.hpp>

namespace tupai
{
	bool cmp_str(const char* str0, const char* str1)
	{
		for (umem i = 0; str0[i] != '\0' || str1[i] != '\0'; i ++)
		{
			if (str0[i] != str1[i])
				return false;
		}
		return true;
	}

	int prompt()
	{
		tty_write_str("Type 'help' for more information\n");

		while (true)
		{
			tty_set_fg_color(0x3);
			tty_write_str("kernel");
			tty_set_fg_color(0xF);
			tty_write('@');
			tty_set_fg_color(0x4);
			tty_write_str("tupai");
			tty_set_fg_color(0xF);
			tty_write_str("> ");

			const umem INPUT_BUFFER_LENGTH = 1024;
			char buffer[INPUT_BUFFER_LENGTH];
			umem buffer_pos = 0;
			while (true)
			{
				char input_char = libk::getchar();

				if (input_char != '\0')
				{
					if (input_char == '\n' || buffer_pos == INPUT_BUFFER_LENGTH - 1) // Newline or end of buffer
					{
						buffer[buffer_pos] = '\0';
						break;
					}
					else if (input_char == '\b') // Backspace
					{
						if (buffer_pos > 0)
							buffer_pos --;
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
				tty_write_str("help      - Show this help screen\n");
				tty_write_str("sys       - Show system information\n");
				tty_write_str("snake     - Play a demo snake game\n");
				tty_write_str("adventure - Play an adventure game\n");
				tty_write_str("timer     - Timer test program\n");
				tty_write_str("exit      - Close the prompt session\n");
				tty_write_str("clear     - Clear the screen\n");
				tty_write_str("abort     - Abort the system\n");
			}
			else if (cmp_str(buffer, "sys"))
			{
				prog::sys_main(0, nullptr);
			}
			else if (cmp_str(buffer, "timer"))
			{
				prog::timer_main(0, nullptr);
			}
			else if (cmp_str(buffer, "exit"))
			{
				return 0;
			}
			else if (cmp_str(buffer, "abort"))
			{
				tty_write_str("Aborting...\n");
				libk::abort();
			}
			else if (cmp_str(buffer, "snake"))
			{
				prog::snake_main(0, nullptr);
			}
			else if (cmp_str(buffer, "adventure"))
			{
				prog::adventure_main(0, nullptr);
			}
			else if (cmp_str(buffer, "clear"))
			{
				tty_clear();
			}
			else
			{
				if (libk::strlen(buffer) > 0)
				{
					tty_write_str(buffer);
					tty_write_str(": command not found\n");
				}
			}
		}

		return 1;
	}
}
