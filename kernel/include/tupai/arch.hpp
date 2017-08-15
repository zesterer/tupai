//
// file : arch.hpp
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

#ifndef TUPAI_ARCH_HPP
#define TUPAI_ARCH_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	const size_t ARCH_PAGE_SIZE = 4096;
	const size_t ARCH_PAGE_SIZE_P2 = 12;

	extern "C" char kernel_start[];
	extern "C" char kernel_end[];

	struct arch_reg_state;
	size_t arch_get_reg_state_size();

	const char* arch_get_target();
	const char* arch_get_family();
	size_t arch_get_offset();

	size_t arch_get_kernel_start();
	size_t arch_get_kernel_end();

	void  arch_init();

	void  arch_display_reg_state(arch_reg_state* state);
}

#endif
