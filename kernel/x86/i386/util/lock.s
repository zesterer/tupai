//
// file : lock.s
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

.global spinlock_lock_impl
.global spinlock_unlock_impl

.section .text.boot

	spinlock_lock_impl:
		mov 4(%esp), %edx // Get byte location from stack

		mov $1, %eax

		_lock:
			xchg (%edx), %eax // Perform exchange

			test %eax, %eax
			jnz preempt // If the mutex was not 0 (i.e: already locked) jump back to the lock procedure (TODO: pre-empt instead)

		ret

		preempt:
			//int $0x80
			jmp _lock

	spinlock_unlock_impl:
		mov 4(%esp), %edx // Get byte location from stack

		mov $0, %eax
		xchg (%edx), %eax // Perform exchange

		//int $0x80

		ret
