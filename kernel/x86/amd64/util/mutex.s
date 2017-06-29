//
// file : mutex.s
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

.global mutex_lock_impl
.global mutex_unlock_impl

.section .text

	mutex_lock_impl:

		mov $1, %rax

	_lock:
			xchg (%rdi), %rax // Perform exchange

			test %rax, %rax
			jnz preempt // If the mutex was not 0 (i.e: already locked) jump back to the lock procedure (TODO: pre-empt instead)

		ret

	preempt:
		int $0x80

	mutex_unlock_impl:

		mov $0, %rax
		xchg (%rdi), %rax // Perform exchange

		ret
