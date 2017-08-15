//
// file : call.hpp
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

#ifndef TUPAI_SYS_CALL_HPP
#define TUPAI_SYS_CALL_HPP

// Tupai

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		const uint8_t CALL_IRQ = 0x80;

		enum class CALL
		{
			// Process control
			YIELD  = 0x100,
			LMUTEX = 0x101, // temporary
			UMUTEX = 0x102, // temporary
			// and more...

			// File descriptor creation
			OPEN   = 0x200,
			PIPE   = 0x201,
			SOCKET = 0x202,
			// and more...

			// File descriptor destruction
			CLOSE = 0x300,
			// and more...

			// File manipulation
			READ  = 0x400,
			WRITE = 0x401,
			SEEK  = 0x402,
			// and more...
		};

		void call_init();

		void call(CALL call, size_t arg0 = 0, size_t arg1 = 0, size_t arg2 = 0);
	}
}

#endif
