//
// file : kentry.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/common/kmain.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		namespace amd64
		{
			extern "C" void x86_amd64_kentry(uint32_t mb_header, uint32_t stack)
			{
				// Declare as unused
				(void) mb_header;
				(void) stack;

				kmain();
			}
		}
	}
}
