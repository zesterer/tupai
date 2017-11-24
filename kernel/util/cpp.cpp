//
// file : cpp.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

#include <tupai/util/cpp.hpp>

#if defined(ARCH_FAMILY_x86)
	extern "C" int  __cxa_atexit(void (*destructor) (void*) __attribute__ ((unused)), void* arg __attribute__ ((unused)), void* dso __attribute__ ((unused))) { return 0; }
	extern "C" void __cxa_finalize(void* f __attribute__ ((unused))) { }
#endif

#if defined(ARCH_FAMILY_arm)
	extern "C" int  __aeabi_atexit(void* obj __attribute__ ((unused)), void (*destructor) (void*) __attribute__ ((unused)), void* dso __attribute__ ((unused))) { return 0; }
	extern "C" void __aeabi_finalize(void* f __attribute__ ((unused))) { }
#endif
