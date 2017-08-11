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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/mem/kmem.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

extern "C" void* memset(void* ptr, int value, size_t num)
{
	for (size_t i = 0; i < num; i ++)
		((uint8_t*)ptr)[i] = (uint8_t)value;
	return ptr;
}

extern "C" void* memcpy(void* dest, const void* src, size_t n)
{
	for (size_t i = 0; i < n; i ++)
		((uint8_t*)dest)[i] = ((const uint8_t*)src)[i];
	return dest;
}

void* operator new(size_t n)
{
	return tupai::mem::kmem::alloc(n);
}

void* operator new[](size_t n)
{
	return tupai::mem::kmem::alloc(n);
}

void operator delete(void* ptr)
{
	tupai::mem::kmem::dealloc(ptr);
}

void operator delete[](void* ptr)
{
	tupai::mem::kmem::dealloc(ptr);
}

void operator delete(void* ptr, size_t size __attribute__ ((unused)))
{
	tupai::mem::kmem::dealloc(ptr);
}

void operator delete[](void* ptr, size_t size __attribute__ ((unused)))
{
	tupai::mem::kmem::dealloc(ptr);
}

//void* __dso_handle;

#if defined(ARCH_FAMILY_x86)
	extern "C" int  __cxa_atexit(void (*destructor) (void*) __attribute__ ((unused)), void* arg __attribute__ ((unused)), void* dso __attribute__ ((unused))) { return 0; }
	extern "C" void __cxa_finalize(void* f __attribute__ ((unused))) { }
#endif

#if defined(ARCH_FAMILY_arm)
	extern "C" int  __aeabi_atexit(void* obj __attribute__ ((unused)), void (*destructor) (void*) __attribute__ ((unused)), void* dso __attribute__ ((unused))) { return 0; }
	extern "C" void __aeabi_finalize(void* f __attribute__ ((unused))) { }
#endif
