//
// file : call.cpp
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

// Tupai
#include <tupai/sys/call.hpp>
#include <tupai/interrupt.hpp>

namespace tupai
{
	namespace sys
	{
		void call(CALL call, size_t arg0, size_t arg1, size_t arg2)
		{
			if (interrupt_enabled())
			{
				asm volatile (
					"mov %0, %%eax \n\
					 mov %1, %%ebx \n\
					 mov %2, %%ecx \n\
					 mov %3, %%edx \n\
					 int $0x80"
					:
					: "m"((size_t)call), "m"(arg0), "m"(arg1), "m"(arg2)
					: "%eax", "%ebx", "%ecx", "%edx"
				);
			}
		}
	}
}
