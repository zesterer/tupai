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
#include <tupai/util/out.hpp>

#include <tupai/util/conv.hpp>

#include <tupai/tty.hpp>

namespace tupai
{
	namespace util
	{
		void printc(char c)
		{
			tty_write(c);
		}

		void print(const char* str)
		{
			tty_write_str(str);
		}

		void println(const char* str)
		{
			tty_write_str(str);
			tty_write('\n');
		}

		void printf(const char* fmt, ...)
		{
			__builtin_va_list args;
			__builtin_va_start(args, fmt);

			bool escaped = false;
			bool escape_blocked = false;
			for (umem i = 0; fmt[i] != '\0';)
			{
				char c = fmt[i];
				bool repeat = false;

				if (escaped)
				{
					switch (c)
					{
					case 's':
						tty_write_str(__builtin_va_arg(args, const char*));
						break;
					case 'c':
						tty_write((char)__builtin_va_arg(args, int));
						break;
					case 'd':
					case 'i':
						tty_write_str(util::compose(__builtin_va_arg(args, int32), 10).val().raw());
						break;
					case 'u':
						tty_write_str(util::compose(__builtin_va_arg(args, uint32), 10).val().raw());
						break;
					case 'X':
						tty_write_str(util::compose(__builtin_va_arg(args, int32), 16).val().raw());
						break;
					case 'o':
						tty_write_str(util::compose(__builtin_va_arg(args, int32), 8).val().raw());
						break;

					// Unimplemented
					case 'x':
						{
							__builtin_va_arg(args, int);
							tty_write_str("NAN");
						}
						break;

					default:
						repeat = true;
						break;
					}

					escaped = false;
				}
				else
				{
					switch (c)
					{
					case '%':
						{
							if (!escape_blocked)
								escaped = true;
							else
							{
								tty_write(c);
								escape_blocked = false;
							}
						}
						break;

					case '\\':
						escape_blocked = true;
						break;

					default:
						{
							tty_write(c);
							escape_blocked = false;
						}
						break;
					}
				}

				if (!repeat)
					i ++;
			}

			__builtin_va_end(args);
		}
	}
}
