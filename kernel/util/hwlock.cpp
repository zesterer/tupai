//
// file : mutex.cpp
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
#include <tupai/util/hwlock.hpp>
#include <tupai/irq.hpp>
#include <tupai/cpu.hpp>

namespace tupai
{
	namespace util
	{
		static volatile int hwlock_ref = 0;
		static volatile bool irq_enabled = false;

		void hwlock_acquire()
		{
			if (cpu::is_irq())
				return;

			bool irq = irq::are_enabled();
			irq::disable();

			if (hwlock_ref == 0)
				irq_enabled = irq;
			else if (irq_enabled)
				hwlock_ref ++;
		}

		void hwlock_release()
		{
			if (cpu::is_irq())
				return;

			if (irq_enabled)
			{
				hwlock_ref --;
				if (hwlock_ref == 0)
					irq::enable();
			}
		}
	}
}
