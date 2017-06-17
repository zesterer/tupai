//
// file : kbd.cpp
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
#include <tupai/dev/kbd.hpp>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/kbd.hpp>
#else
	#warning "Architecture provides no keyboard device!"
#endif

namespace tupai
{
	namespace dev
	{
		static bool kbd_initiated = false;

		void kbd_init()
		{
			if (kbd_initiated)
				return;

			#if defined(ARCH_FAMILY_x86)
				x86::kbd_init();
			#endif

			kbd_initiated = true;
		}
	}
}
