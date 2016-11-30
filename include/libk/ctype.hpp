/*
* 	file : ctype.hpp
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

#ifndef _CTYPE_HPP
#define _CTYPE_HPP

// Tupai
#include <tupai/type.hpp>

namespace libk
{
	/* Character Classification Functions */

	bool isalnum(sint c);
	bool isalpha(sint c);
	bool isblank(sint c);
	bool iscntrl(sint c);
	bool isdigit(sint c);
	bool isgraph(sint c);
	bool islower(sint c);
	bool isprint(sint c);
	bool ispunct(sint c);
	bool isspace(sint c);
	bool isupper(sint c);
	bool isxdigit(sint c);

	/* Character Conversion Functions */

	sint toupper(sint c);
	sint tolower(sint c);
}

#endif
