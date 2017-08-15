//
// file : mutex.s
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

.global mutex_lock_impl
.global mutex_unlock_impl

.section .text

	mutex_lock_impl:

		mov r1, #1

	_lock:
			ldrex r2, [r0]

			cmp r2, r1
			beq _lock // Branch back to _lock if it's locked. TODO : Make this branch to 'preempt'.

			strexne r2, r1, [r0] // Attempt to lock mutex

			cmpne r2, #1 // Test mutex to check lock
			beq _lock // Locking failed: retry

			// Data memory barrier
			mov r0, #0
			mcr p15, #0, r0, c7, c10, #5

			bx lr // Return

	//preempt:
	//	int $0x80

	mutex_unlock_impl:

		mov r1, #0

		// Data memory barrier
		mov r0, #0
		mcr p15, #0, r0, c7, c10, #5

		str r1, [r0] // Unlock mutex

		bx lr // Return
