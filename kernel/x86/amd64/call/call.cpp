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
#include <tupai/call/call.hpp>

namespace tupai
{
	namespace call
	{
		size_t call(size_t func, size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			size_t status;
			asm volatile (
				"mov %1, %%rax \n\
				 mov %2, %%rbx \n\
				 mov %3, %%rcx \n\
				 mov %4, %%rdx \n\
				 mov %5, %%rdi \n\
				 int $0x80     \n\
				 mov %%rax, %0"
				: "=m"(status)
				: "m"(func), "m"(arg0), "m"(arg1), "m"(arg2), "m"(arg3)
				: "%rax", "%rbx", "%rcx", "%rdx", "%rdi"
			);
			return status;
		}
	}
}
