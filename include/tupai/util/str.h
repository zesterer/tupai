//
// file : str.h
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_UTIL_STR_H
#define TUPAI_UTIL_STR_H

// Standard
#include <stddef.h>
#include <stdint.h>

static inline size_t str_len(const char* str)
{
	size_t i;
	for (i = 0; str[i] != '\0'; i ++);
	return i;
}

static inline void str_cpy(const char* src, char* dest)
{
	size_t i;
	for (i = 0; src[i] != '\0'; i ++)
		dest[i] = src[i];
	dest[i] = '\0';
}

#endif
