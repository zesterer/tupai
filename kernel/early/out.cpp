/*
* 	file : out.cpp
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
#include <tupai/early/out.hpp>
#include <tupai/early/ansi.hpp>
#include <tupai/console.hpp>

namespace tupai
{
	namespace early
	{
		void printchar(char c)
		{
			safeptr<console> g_console = console_get_global();
			if (g_console.is_valid())
				g_console.val()->write_char(c);

			if (ansi_get_initiated())
				ansi_handle(c);
		}

		void print(const char* string)
		{
			if (ansi_get_initiated())
			{
				for (umem i = 0; string[i] != '\0'; i ++)
					ansi_handle(string[i]);
			}
		}
	}
}
