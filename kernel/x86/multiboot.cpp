//
// file : multiboot.cpp
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
#include <tupai/x86/multiboot.hpp>
#include <tupai/debug.hpp>

namespace tupai
{
	namespace x86
	{
		static const uint64_t MULTIBOOT_EAX_MAGIC = 0x36d76289;

		void multiboot_set_header(uint64_t magic, void* header)
		{
			if (magic != MULTIBOOT_EAX_MAGIC)
			{
				debug_println("Warning: Multiboot magic constant is not correct!");
				return;
			}
		}
	}
}
