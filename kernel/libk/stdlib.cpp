/*
* 	file : stdlib.cpp
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
#include <libk/stdlib.hpp>

// Tupai
#include <tupai/kdebug.hpp>
#include <tupai/mempool.hpp>

namespace libk
{
	/* String Conversion */

	double atof(const char* str);

	sint atoi(const char* str)
	{
		int sign = 1;
		int total = 0;

		for (size_t i = 0; str[i] != '\0'; i ++)
		{
			if (str[i] == '-')
				sign *= -1;
			else if (str[i] == '+')
				sign *= 1;
			else if (str[i] >= '0' && str[i] <= '9')
			{
				total = (total * 10) + (str[i] - '0');
			}
			else
				return 0;
		}

		return total * sign;
	}

	slong  atol(const char* str);
	squad  atoll(const char* str);
	double strtod(const char* str, char** endptr);
	float  strtof(const char* str, char** endptr);
	slong  strtol(const char* str, char** endptr, sint base);
	squad  strtoll(const char* str, char** endptr, sint base);
	ulong strtoul(const char* str, char** endptr, sint base);
	uquad  strtoull(const char* str, char** endptr, sint base);

	char* itoa(sint value, char* str, sint base)
	{
		slong v = 1;
		int i = 0;

		if (str == nullptr)
			str = (char*)malloc(sizeof(char) * (sizeof(sint) * 8 + 2));

		if (base < 2 || base > 32)
		{
			str[0] = '\0';
			return str;
		}

		if (value < 0)
		{
			str[i] = '-';
			i ++;
		}

		value = abs(value);

		while (v < (slong)((ulong)(-1) / 4) / base)
			v *= base;

		while (v > 0)
		{
			if (value / v > 0)
			{
				if (((value % (v * base)) / v) % base >= 10)
					str[i] = 'A' + (value / v) % base - 10;
				else
					str[i] = '0' + (value / v) % base;

				i ++;
			}
			v /= base;
		}

		if (value == 0)
		{
			str[i] = '0';
			i ++;
		}

		str[i] = '\0';
		return str;
	}

	/* Pseudo-Random Sequence Generation */

	ulong rand_next = 1;

	sint rand()
	{
		rand_next = rand_next * 1103515245 + 12345;
		return (rand_next % 65536) - 32768;
	}

	void srand(uint seed)
	{
		rand_next = seed;
	}

	/* Dynamic Memory Management */

	void* calloc(umem num, umem size)
	{
		void* loc = tupai::mempool_alloc(num * size);

		for (umem i = 0; i < num * size; i ++)
			((byte*)loc)[i] = 0x0;

		return loc;
	}

	void* malloc(umem size)
	{
		return tupai::mempool_alloc(size);
	}

	void free(void* ptr)
	{
		tupai::mempool_dealloc(ptr);
	}

	void* realloc(void* ptr, umem size)
	{
		return tupai::mempool_realloc(ptr, size);
	}

	/* Environment */

	// Must be at least 32
	const umem EXIT_FUNC_MAX = 32;
	void (*exit_func[EXIT_FUNC_MAX])();
	umem exit_func_index = 0;

	// Must be at least 32
	const umem QUICK_EXIT_FUNC_MAX = 32;
	void (*quick_exit_func[EXIT_FUNC_MAX])();
	umem quick_exit_func_index = 0;

	void abort()
	{
		tupai::kpanic("Kernel abort");
	}

	sint atexit(void (*func)())
	{
		if (exit_func_index >= EXIT_FUNC_MAX - 1)
			return 1;

		exit_func_index ++;
		exit_func[exit_func_index] = func;

		return 0;
	}

	sint at_quick_exit(void (*func)())
	{
		if (quick_exit_func_index >= QUICK_EXIT_FUNC_MAX - 1)
			return 1;

		quick_exit_func[quick_exit_func_index] = func;
		quick_exit_func_index ++;

		return 0;
	}

	void exit(sint status __attribute__ ((unused)))
	{
		for (; exit_func_index > 0; exit_func_index --)
		{
			if (exit_func[exit_func_index] != nullptr)
				exit_func[exit_func_index]();
		}

		tupai::kpanic("Kernel 'exit' called");
	}

	char* getenv(const char* name);

	void quick_exit(sint status __attribute__ ((unused)))
	{
		for (; quick_exit_func_index > 0; quick_exit_func_index --)
		{
			if (quick_exit_func[quick_exit_func_index] != nullptr)
				quick_exit_func[quick_exit_func_index]();
		}

		tupai::kpanic("Kernel 'quick_exit' called");
	}

	sint  system(const char* string);
	void  _Exit(sint status);

	/* Searching And Sorting */

	void* bsearch(const void* key, const void* base, umem nitems, umem size, sint (*compar)(const void*, const void*));
	void  qsort(void* base, umem nitems, umem size, sint (*compar)(const void*, const void*));

	/* Integer Arithmetics */

	sint abs(sint n)
	{
		return n < 0 ? -n : n;
	}

	div_t   div(sint number, sint denom);

	slong labs(slong n)
	{
		return n < 0 ? -n : n;
	}

	ldiv_t  ldiv(slong numer, slong denom);

	squad llabs(squad n)
	{
		return n < 0 ? -n : n;
	}

	lldiv_t lldiv(squad numer, squad denom);

	/* Multibyte Characters */

	sint mblen(const char* str, umem n);
	sint mbtowc(wchar_t* pwcs, const char* str, umem n);
	sint wctomb(char* str, wchar_t wc);

	/* Multibyte Strings */

	umem mbstowcs(uint8_t* pwcs, const char* str, umem n);
	umem wcstombs(char* str, const uint8_t* pwcs, umem n);
}
