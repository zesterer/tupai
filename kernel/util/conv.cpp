/*
* 	file : conv.cpp
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
#include <tupai/util/conv.hpp>
#include <tupai/kdebug.hpp>

// Libk
#include <libk/ctype.hpp>
#include <libk/stdio.hpp>

// GCC
#include <stdint.h>

namespace tupai
{
	template<typename T>
	static safeval<T> parse_integer_generic(const char* str, umem n, bool is_signed = true)
	{
		enum class stage { START, SIGN, PREFIX, DIGITS, FINISH, INVALID };

		stage state = stage::START;
		char c = str[0];
		bool cancel_incr = false; // To cancel a character increment
		bool end = false;
		bool prefix_start = false;
		int base = 10;
		bool started_digits = false;

		T sign = 1;
		T val = 0;

		for (umem count = 0; count <= n && !end;)
		{
			if (!cancel_incr) // Cancel increment allows a state change to occur without missing a character
			{
				c = str[count];
				count ++;
			}
			else
				cancel_incr = false;

			if (count == n) // If we've got to the end of the section, fake a string ending
				c = '\0';

			switch (state)
			{
			case stage::START:
				{
					if (c == '\0')
					{
						end = true;
						state = stage::INVALID;
					}
					else if (!libk::isblank(c))
					{
						cancel_incr = true;
						state = stage::SIGN;
					}
				}
				break;

			case stage::SIGN:
				{
					if (c == '+')
					{
						sign = 1;
						state = stage::PREFIX;
					}
					else if (c == '-')
					{
						sign = -1;
						state = stage::PREFIX;
					}
					else
					{
						cancel_incr = true;
						state = stage::PREFIX;
					}
				}
				break;

			case stage::PREFIX:
				{
					if (prefix_start)
					{
						if (c == 'x')
							base = 16;
						else if (c == 'b')
							base = 2;
						else if (c == 'd')
							base = 10;
						else
							{
								cancel_incr = true;
								state = stage::DIGITS;
							}
						state = stage::DIGITS;
					}
					else if (c == '0')
					{
						prefix_start = true;
					}
					else
					{
						cancel_incr = true;
						state = stage::DIGITS;
					}
				}
				break;

			case stage::DIGITS:
				{
					if (c == '\0' && started_digits)
					{
						end = true;
						state = stage::FINISH;
					}
					else if (started_digits && libk::isblank(c))
						state = stage::FINISH;
					else
					{
						switch (base)
						{
						case 10:
							{
								if (libk::isdigit(c))
								{
									val *= 10;
									val += c - '0';
									started_digits = true;
								}
								else
									state = stage::INVALID;
							}
							break;

						case 16:
							{
								if (libk::isxdigit(c))
								{
									val *= 16;
									val += (c >= '0' && c <= '9') ? (c - '0') : ((c >= 'A' && c <= 'F') ? (c + 0xA - 'A') : (c + 0xA - 'a'));
									started_digits = true;
								}
								else
									state = stage::INVALID;
							}
							break;

						case 2:
							{
								if (c == '0' || c == '1')
								{
									val *= 2;
									val += c - '0';
									started_digits = true;
								}
								else
									state = stage::INVALID;
							}
							break;

						default:
							kpanic("Unsupported integer parse base");
							break;
						};
					}
				}
				break;

			case stage::FINISH:
				{
					if (c == '\0')
						end = true;
					else if (!libk::isblank(c))
						state = stage::INVALID;
				}
				break;

			case stage::INVALID:
				end = true;
				break;

			default:
				kpanic("Integer parse state invalid");
				break;
			}
		}

		if (state != stage::FINISH)
			return safeval<T>(0, false);
		else
		{
			if (!is_signed && sign != 1)
				return safeval<T>(0, false);
			else
				return safeval<T>(sign * val, true);
		}
	}

	template<> safeval<int8> parse<int8>(const char* str, umem n) { return parse_integer_generic<int8>(str, n); }
	template<> safeval<uint8> parse<uint8>(const char* str, umem n) { return parse_integer_generic<uint8>(str, n); }
	template<> safeval<int16> parse<int16>(const char* str, umem n) { return parse_integer_generic<int16>(str, n); }
	template<> safeval<uint16> parse<uint16>(const char* str, umem n) { return parse_integer_generic<uint16>(str, n); }
	template<> safeval<int32> parse<int32>(const char* str, umem n) { return parse_integer_generic<int32>(str, n); }
	template<> safeval<uint32> parse<uint32>(const char* str, umem n) { return parse_integer_generic<uint32>(str, n); }
	template<> safeval<int64> parse<int64>(const char* str, umem n) { return parse_integer_generic<int64>(str, n); }
	template<> safeval<uint64> parse<uint64>(const char* str, umem n) { return parse_integer_generic<uint64>(str, n); }
}
