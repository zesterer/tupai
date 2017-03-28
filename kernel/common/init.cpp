//
// file : init.cpp
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
#include <tupai/common/init.hpp>

#if defined(ARCH_amd64)
	#include <tupai/x86/amd64/init.hpp>
#elif defined(ARCH_i386)
	#include <tupai/x86/i386/init.hpp>
#elif defined(ARCH_rpi2)
	#include <tupai/arm/rpi2/init.hpp>
#endif

namespace tupai
{
	int init()
	{
		#if defined(ARCH_amd64)
			return amd64_init();
		#elif defined(ARCH_i386)
			return i386_init();
		#elif defined(ARCH_rpi2)
			return rpi2_init();
		#endif
	}
}
