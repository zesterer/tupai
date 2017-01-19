//
//	file : mutex.s
//
// 	This file is part of Tupai.
//
//	Tupai is free software: you can redistribute it and/or modify
// 	it under the terms of the GNU General Public License as published by
// 	the Free Software Foundation, either version 3 of the License, or
// 	(at your option) any later version.
//
// 	Tupai is distributed in the hope that it will be useful,
// 	but WITHOUT ANY WARRANTY; without even the implied warranty of
// 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 	GNU General Public License for more details.
//
// 	You should have received a copy of the GNU General Public License
// 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

.global _mutex_lock
.global _mutex_unlock

.section .text

	_mutex_lock:
		mov 4(%esp), %ebx // Get byte location from stack
		mov $1, %eax

		xchg (%ebx), %eax

		test %eax, %eax
		jnz _mutex_lock // If the mutex was not 0 (i.e: already locked) jump back to the lock procedure

		ret

	_mutex_unlock:
		mov 4(%esp), %ebx // Get byte location from stack
		mov $0, %eax

		xchg (%ebx), %eax

		ret
