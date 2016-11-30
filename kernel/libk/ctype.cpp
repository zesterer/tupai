/*
* 	file : ctype.cpp
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

// Libk
#include <libk/ctype.hpp>

namespace libk
{
	/* Character Classification Functions */

	bool isalnum(sint c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
	}

	bool isalpha(sint c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	bool isblank(sint c)
	{
		return c == '\t' || c == ' ';
	}

	bool iscntrl(sint c)
	{
		return (c >= 0x00 && c <= 0x1F) || c == 0x7F;
	}

	bool isdigit(sint c)
	{
		return c >= '0' && c <= '9';
	}

	bool isgraph(sint c)
	{
		return isprint(c) && c != ' ';
	}

	bool islower(sint c)
	{
		return c >= 'a' && c <= 'z';
	}

	bool isprint(sint c)
	{
		return c >= 0x20 && c <= 0x7F;
	}

	bool ispunct(sint c)
	{
		return isprint(c) && !isalnum(c);
	}

	bool isspace(sint c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
	}

	bool isupper(sint c)
	{
		return (c >= 'A' && c <= 'Z');
	}

	bool isxdigit(sint c)
	{
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
	}

	/* Character Conversion Functions */

	sint toupper(sint c)
	{
		return (c >= 'a' && c <= 'z') ? ('A' + c - 'a') : c;
	}

	sint tolower(sint c)
	{
		return (c >= 'A' && c <= 'Z') ? ('a' + c - 'A') : c;
	}
}
