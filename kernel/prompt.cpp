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

// Libk
#include <libk/stdlib.hpp>

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

	int r = 1337;
	int rand()
	{
		r = (r * 1337 - r) ^ r;
		return r;
	}

	void writeint(int i)
	{
		tty_write_str(libk::itoa(i));
		/*tty_write('0' + (i / 100000) % 10);
		tty_write('0' + (i / 10000) % 10);
		tty_write('0' + (i / 1000) % 10);
		tty_write('0' + (i / 100) % 10);
		tty_write('0' + (i / 10) % 10);
		tty_write('0' + (i / 1) % 10);*/
	}

	int snake()
	{
		int dx = 1;
		int dy = 0;
		int sx = 0;
		int sy = 0;

		int slen = 0;
		int body_x[256];
		int body_y[256];
		int score = 0;

		int foodx = libk::abs(rand()) % 80;
		int foody = libk::abs(rand()) % 24;

		uint32 gap = 20000000;
		while (true)
		{
			if (slen < 4)
				slen ++;

			score ++;
			volatile uint32 i = 0;
			for (i = 0; i < gap; i ++);

			char k = key_char;
			key_char = '\0';
			if (k == 'w' && dy <= 0) { dx = +0; dy = -1; }
			if (k == 'a' && dx <= 0) { dx = -1; dy = +0; }
			if (k == 's' && dy >= 0) { dx = +0; dy = +1; }
			if (k == 'd' && dx >= 0) { dx = +1; dy = +0; }
			if (k == 'i') gap *= 2;
			if (k == 'k') gap /= 2;
			if (k == '\b') break;

			sx += dx;
			sy += dy;

			sx = (sx + 80) % 80;
			sy = (sy + 24) % 24;

			if (foodx == sx && foody == sy)
			{
				foodx = libk::abs(rand()) % 80;
				foody = libk::abs(rand()) % 24;
				score += 500;
				slen ++;
			}

			// Graphics
			body_x[0] = sx;
			body_y[0] = sy;

			for (int i = 256 - 2; i >= 0; i --)
			{
				body_x[i + 1] = body_x[i];
				body_y[i + 1] = body_y[i];
			}

			bool gameover = false;
			for (int i = 2; i < slen && i < 256; i ++)
			{
				if (body_x[i] == sx && body_y[i] == sy)
					gameover = true;
			}
			if (gameover)
				break;

			tty_clear();
			for (int i = 0; i < slen; i ++)
			{
				tty_place_cursor(body_x[i], body_y[i] + 1);
				tty_write('@');
			}
			tty_place_cursor(foodx, foody + 1);
			tty_write('#');

			// Banner
			tty_place_cursor(0, 0);
			tty_set_bg_color(0x2);
			for (int i = 0; i < 80; i ++) tty_write(' ');
			tty_place_cursor(0, 0);
			tty_write_str("Snake    Score: ");
			writeint(score);
			tty_set_bg_color(0x0);
		}

		tty_place_cursor(0, 24);
		tty_write_str("Game Over!\n");
		tty_write_str("Score: ");
		writeint(score);
		tty_write('\n');
		return 0;
	}

	int prompt()
	{
		tty_write_str("Type 'help' for more information\n");

		while (true)
		{
			//tty_clear();
			tty_write_str("");
			tty_set_fg_color(0x4);
			tty_write_str("tupai");
			tty_set_fg_color(0xF);
			tty_write_str("> ");

			const umem INPUT_BUFFER_LENGTH = 1024;
			char buffer[INPUT_BUFFER_LENGTH];
			umem buffer_pos = 0;
			while (true)
			{
				char input_char = key_char;
				key_char = '\0';

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
				tty_write_str("snake - Play a demo snake game\n");
				tty_write_str("exit  - Close the prompt session\n");
				tty_write_str("clear - Clear the screen\n");
				tty_write_str("abort - Abort the system\n");
			}
			else if (cmp_str(buffer, "sys"))
			{
				tty_write_str(SYSTEM_NAME_DECORATIVE);
				tty_write_str(" ");
				tty_write_str(SYSTEM_VERSION_MAJOR);
				tty_write_str(".");
				tty_write_str(SYSTEM_VERSION_MINOR);
				tty_write_str(".");
				tty_write_str(SYSTEM_VERSION_RELEASE);
				tty_write_str(" on ");
				tty_write_str(SYSTEM_ARCH);
				tty_write('\n');
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
				snake();
			}
			else if (cmp_str(buffer, "clear"))
			{
				tty_clear();
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
