/*
* 	file : kdebug.cpp
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
#include <tupai/kdebug.hpp>
#include <tupai/tty.hpp>

#include <tupai/i686/port.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	bool __kassert__(bool expr, const char* expr_str, int line, const char* func, const char* file)
	{
		if (!expr)
		{
			libk::puts("Warning: assertion '");
			libk::puts(expr_str);
			libk::puts("' at line ");

			bool start = false;
			for (int pow = 1000000; line > 0; pow /= 10)
			{
				if (line >= pow)
				{
					libk::putchar('0' + (line / pow) % 10);
					line = line % pow;
					start = true;
				}
				else if (start)
					libk::putchar('0');
			}

			libk::puts(" in function '");
			libk::puts(func);
			libk::puts("' in file '");
			libk::puts(file);
			libk::puts("' failed!\n");
		}

		return expr;
	}
}
