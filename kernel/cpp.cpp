//
// file : cpp.cpp
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

// Tupai
#include <tupai/sys/kmem.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

extern "C" void* memset(void* ptr, int value, size_t num)
{
	for (size_t i = 0; i < num; i ++)
		((uint8_t*)ptr)[i] = (uint8_t)value;
	return ptr;
}

void* operator new(size_t n)
{
	return tupai::sys::kmem_alloc(n);
}

void* operator new[](size_t n)
{
	return tupai::sys::kmem_alloc(n);
}

void operator delete(void* ptr)
{
	tupai::sys::kmem_dealloc(ptr);
}

void operator delete[](void* ptr)
{
	tupai::sys::kmem_dealloc(ptr);
}

void operator delete(void* ptr, size_t size __attribute__ ((unused)))
{
	tupai::sys::kmem_dealloc(ptr);
}

void operator delete[](void* ptr, size_t size __attribute__ ((unused)))
{
	tupai::sys::kmem_dealloc(ptr);
}

void* __dso_handle;

extern "C" int  __cxa_atexit(void (*destructor) (void*), void* arg, void* dso) { return 0; }
extern "C" void __cxa_finalize(void* f) { }
