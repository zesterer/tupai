/*
* 	file : stdlib.hpp
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

#ifndef _STDLIB_HPP
#define _STDLIB_HPP

// Tupai
#include <tupai/type.hpp>

namespace libk
{
	/* Types */

	typedef struct { sint quot; sint rem; } div_t;
	typedef struct { slong quot; slong rem; } ldiv_t;
	typedef struct { squad quot; squad rem; } lldiv_t;

	/* Macros */

	const sint  EXIT_FAILURE = 1;
	const sint  EXIT_SUCCESS = 0;
	const umem  MB_CUR_MAX = 4;
	const uint RAND_MAX = 2147483647;

	/* String Conversion */

	double atof(const char* str);
	sint   atoi(const char* str);
	slong  atol(const char* str);
	squad  atoll(const char* str);
	double strtod(const char* str, char** endptr);
	float  strtof(const char* str, char** endptr);
	slong  strtol(const char* str, char** endptr, sint base);
	squad  strtoll(const char* str, char** endptr, sint base);
	ulong strtoul(const char* str, char** endptr, sint base);
	uquad  strtoull(const char* str, char** endptr, sint base);

	char* itoa(sint value, char* str, sint base);

	/* Pseudo-Random Sequence Generation */

	sint rand();
	void srand(uint seed);

	/* Dynamic Memory Management */

	void* calloc(umem num, umem size);
	void* malloc(umem size);
	void  free(void* ptr);
	void* realloc(void* ptr, umem size);

	/* Environment */

	void  abort();
	sint  atexit(void (*func)(void));
	sint  at_quick_exit(void (*func)(void));
	void  exit(sint status);
	char* getenv(const char* name);
	void  quick_exit(sint status);
	sint  system(const char* string);
	void  _Exit(sint status);

	/* Searching And Sorting */

	void* bsearch(const void* key, const void* base, umem nitems, umem size, sint (*compar)(const void*, const void*));
	void  qsort(void* base, umem nitems, umem size, sint (*compar)(const void*, const void*));

	/* Integer Arithmetics */

	sint    abs(sint n);
	div_t   div(sint number, sint denom);
	slong   labs(slong x);
	ldiv_t  ldiv(slong numer, slong denom);
	squad   llabs(squad x);
	lldiv_t lldiv(squad numer, squad denom);

	/* Multibyte Characters */

	sint mblen(const char* str, umem n);
	sint mbtowc(wchar_t* pwcs, const char* str, umem n);
	sint wctomb(char* str, wchar_t wc);

	/* Multibyte Strings */

	umem mbstowcs(uint8_t* pwcs, const char* str, umem n);
	umem wcstombs(char* str, const uint8_t* pwcs, umem n);
}

#endif
