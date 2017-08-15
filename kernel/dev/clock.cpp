//
// file : clock.cpp
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
#include <tupai/dev/clock.hpp>

#if defined(ARCH_FAMILY_X86)
	#include <tupai/x86/cmos.hpp>
#else
	#warning "Architecture provides no clock device!"
#endif

namespace tupai
{
	namespace dev
	{
		static bool clock_initiated = false;

		void clock_init()
		{
			if (clock_initiated)
				return;

			#if defined(ARCH_FAMILY_X86)
				x86::cmos_init();
			#endif

			clock_initiated = true;
		}

		datetime_t clock_read()
		{
			#if defined(ARCH_FAMILY_X86)
				return x86::cmos_read();
			#else
			{
				datetime_t time;

				time.sec  = 0;
				time.min  = 0;
				time.hour = 0;

				time.day   = 1;
				time.month = 1;
				time.year  = 2017;

				return time;
			}
			#endif
		}
	}
}
