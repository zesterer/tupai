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
		for (umem i = 0; str0[i] != '\0' || str1[i] != '\0'; i ++)
		{
			if (str0[i] != str1[i])
				return false;
		}
		return true;
	}

	umem str_len(const char* str)
	{
		umem i;
		for (i = 0; str[i] != '\0'; i ++);
		return i;
	}

	int prompt()
	{
		tty_write_str("\nType 'help' for more information\n");

		while (true)
		{
			//tty_clear();
			tty_set_fg_color(0x2);
			tty_write_str("kernel");
			tty_set_fg_color(0xF);
			tty_write_str("@");
			tty_set_fg_color(0x4);
			tty_write_str("tupai");
			tty_set_fg_color(0xF);
			tty_write_str("> ");

			const umem INPUT_BUFFER_LENGTH = 1024;
			char buffer[INPUT_BUFFER_LENGTH];
			umem buffer_pos = 0;
			while (true)
			{
				char input_char = i686::key_char;
				i686::key_char = '\0';

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
				tty_write_str("help  - Show this help screen\n");
				tty_write_str("sys   - Show system information\n");
				tty_write_str("panic - Perform a kernel panic\n");
				tty_write_str("exit  - Close the prompt session\n");
				tty_write_str("halt  - Halt the system\n");
			}
			else if (cmp_str(buffer, "sys"))
			{
				tty_write_str("Tupai 0.1.0 on ");
				tty_write_str(SYSTEM_ARCH);
				tty_write('\n');
			}
			else if (cmp_str(buffer, "panic"))
			{
				kpanic();
			}
			else if (cmp_str(buffer, "exit"))
			{
				return 0;
			}
			else if (cmp_str(buffer, "halt"))
			{
				tty_write_str("Halting...\n");
				khalt();
			}
			else
			{
				if (str_len(buffer) > 0)
				{
					tty_write_str(buffer);
					tty_write_str(": command not found\n");
				}
			}
		}

		return 1;
	}
}
