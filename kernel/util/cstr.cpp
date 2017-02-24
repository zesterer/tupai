/*
* 	file : cstr.cpp
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
#include <tupai/util/cstr.hpp>

namespace tupai
{
	namespace util
	{
		bool cstr_equal(const char* a, const char* b)
		{
			for (umem i = 0; a[i] != '\0' || b[i] != '\0'; i ++)
			{
				if (a[i] != b[i])
					return ((int)a[i] - (int)b[i]) == 0;
			}

			return true;
		}

		void cstr_copy(const char* src, char* dest, umem max)
		{
			umem i;
			for (i = 0; (max == 0 || i < max) && src[i] != '\0'; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		void cstr_append(const char* src, char* dest, umem max)
		{
			for (; dest[0] != '\0'; dest += sizeof(char));
			umem i;
			for (i = 0; (max == 0 || i < max) && src[i] != '\0'; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		umem cstr_length(const char* a)
		{
			umem i;
			for (i = 0; a[i] != '\0'; i ++);
			return i;
		}
	}
}
