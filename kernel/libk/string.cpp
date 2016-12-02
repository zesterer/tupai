/*
* 	file : string.cpp
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
#include <libk/string.hpp>

namespace libk
{
	/* Copying */

	void* memcpy(void* destination, const void* source, umem num);
	void* memmove(void* destination, const void* source, umem num);
	char* strcpy(char* destination, const char* source);
	char* strncpy(char* destination, const char* source, umem num);

	/* Concatenation */

	char* strcat(char* destination, const char* source);
	char* strncat(char* destination, const char* source, umem num);

	/* Comparasion */

	sint memcmp(const void* ptr1, const void* ptr2, umem num);

	sint strcmp(const char* str1, const char* str2)
	{
		for (umem i = 0; str1[i] != '\0' || str2[i] != '\0'; i ++)
		{
			if (str1[i] != str2[i])
				return (sint)str1[i] - (sint)str2[i];
		}

		return 0;
	}

	sint strcoll(const char* str1, const char* str2);
	sint strncmp(const char* str1, const char* str2, umem num);
	umem strxfrm(char* destination, const char* source, umem num);

	/* Searching */

	void* memchr(void* ptr, sint value, umem num);

	/* Other */

	void* memset(void* ptr, sint value, umem num)
	{
		for (umem i = 0; i < num; i ++)
			((byte*)ptr)[i] = (byte)value;

		return ptr;
	}

	char* strerror(sint errnum);

	umem strlen(const char* str)
	{
		umem i;
		for (i = 0; str[i] != '\0'; i ++);
		return i;
	}
}
