/*
* 	file : cpp.cpp
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
#include <tupai/mempool.hpp>

// GCC
#include <stddef.h>

void* operator new(size_t size)
{
	return tupai::mempool_alloc(size);
}

void* operator new[](size_t size)
{
	return tupai::mempool_alloc(size);
}

void operator delete(void* ptr)
{
	tupai::mempool_dealloc(ptr);
}

void operator delete[](void* ptr)
{
	tupai::mempool_dealloc(ptr);
}

void operator delete(void* ptr, unsigned long n __attribute__ ((unused)))
{
	tupai::mempool_dealloc(ptr);
}

void operator delete[](void* ptr, unsigned long n __attribute__ ((unused)))
{
	tupai::mempool_dealloc(ptr);
}

void* __dso_handle;
void* __cxa_atexit;


inline uint64_t uint64_div64(uint64_t dividend, uint64_t divisor)
{
	uint64_t shift = divisor;
	uint64_t aux   = divisor;

	while (shift < dividend && (aux <<= 1) > shift)
		shift = aux;

	for (aux = 0; shift >= divisor; shift >>= 1)
	{
		aux <<= 1;
		if (shift <= dividend)
		{
			aux ++;
			dividend -= shift;
		}
	}

	return aux;
}

inline uint64_t __udivdi3(uint64_t a, uint64_t b)
{
	return uint64_div64(a, b);
}

uint64_t uint64_mod64(uint64_t dividend, uint64_t divisor)
{

	uint64_t shift = divisor;
	uint64_t aux   = divisor;

	while (shift < dividend && (aux <<= 1) > shift)
		shift = aux;

	for (aux = 0; shift >= divisor; shift >>= 1)
	{
		aux <<= 1;
		if (shift <= dividend)
		{
			aux ++;
			dividend -= shift;
		}
	}

	return dividend;
}
