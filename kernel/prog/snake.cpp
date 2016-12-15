/*
* 	file : snake.cpp
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
#include <tupai/prog/snake.hpp>

#include <tupai/tty.hpp>

// Libk
#include <libk/stdio.hpp>
#include <libk/stdlib.hpp>

namespace tupai
{
	namespace prog
	{
		int snake_main(int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused)))
		{
			int dx = 1;
			int dy = 0;
			int sx = 0;
			int sy = 0;

			int slen = 0;
			int body_x[256];
			int body_y[256];
			int score = 0;

			int foodx = libk::abs(libk::rand()) % 16;
			int foody = libk::abs(libk::rand()) % 16;

			uint32 gap = 20000000;
			while (true)
			{
				if (slen < 4)
					slen ++;

				score ++;
				volatile uint32 i = 0;
				for (i = 0; i < gap; i ++);

				if (!libk::getisempty())
				{
					char k = libk::getchar();
					if (k == 'w' && dy <= 0) { dx = +0; dy = -1; }
					if (k == 'a' && dx <= 0) { dx = -1; dy = +0; }
					if (k == 's' && dy >= 0) { dx = +0; dy = +1; }
					if (k == 'd' && dx >= 0) { dx = +1; dy = +0; }
					if (k == 'i') gap *= 2;
					if (k == 'k') gap /= 2;
					if (k == '\b') break;
				}

				sx += dx;
				sy += dy;

				sx = (sx + 16) % 16;
				sy = (sy + 16) % 16;

				if (foodx == sx && foody == sy)
				{
					foodx = libk::abs(libk::rand()) % 16;
					foody = libk::abs(libk::rand()) % 16;
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

				int offx = (80 - 16) / 2;
				int offy = (25 - 16) / 2;

				// Background
				for (int yy = -1; yy < 17; yy ++)
				{
					for (int xx = -1; xx < 17; xx ++)
					{
						tty_place_cursor(xx + offx, yy + offy);
						if (xx == -1 || yy == -1 || xx == 16 || yy == 16)
						{
							tty_set_bg_color(0xA);
							tty_write('#');
						}
						else
						{
							tty_set_bg_color(0x9);
							tty_write(' ');
						}
					}
				}
				tty_set_bg_color(0x9);

				for (int i = 0; i < slen; i ++)
				{
					tty_place_cursor(body_x[i] + offx, body_y[i] + offy);
					tty_write('0');
				}

				tty_place_cursor(foodx + offx, foody + offy);
				libk::putchar('@');

				tty_set_bg_color(0x0);

				// Banner
				tty_place_cursor(0, 0);
				tty_set_bg_color(0x2);
				for (int i = 0; i < 80; i ++) tty_write(' ');
				tty_place_cursor(0, 0);
				libk::printf("Snake   Score: %i", score);
				tty_set_bg_color(0x0);
			}

			tty_place_cursor(0, 24);
			tty_write_str("Game Over!\n");
			libk::printf("Score: %i", score);
			libk::putchar('\n');
			return 0;
		}
	}
}
