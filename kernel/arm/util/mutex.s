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
