//
// file : call.cpp
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

// Tupai
#include <tupai/sys/call.hpp>

namespace tupai
{
	namespace sys
	{
		void call(CALL call, size_t arg0, size_t arg1, size_t arg2)
		{
			asm volatile (
				"mov %0, %%eax \n\
				 mov %1, %%ebx \n\
				 mov %2, %%ecx \n\
				 mov %3, %%edx \n\
				 int $0x80"
				:
				: "m"((size_t)call), "m"(arg0), "m"(arg1), "m"(arg2)
				: "%eax", "%ebx", "%ecx", "%edx"
			);
		}
	}
}
