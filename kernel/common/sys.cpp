//
// file : sys.cpp
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
#include <tupai/common/sys.hpp>

#if defined(ARCH_amd64)
	#include <tupai/x86/amd64/arch.hpp>
#elif defined(ARCH_i386)
	#include <tupai/x86/i386/arch.hpp>
#elif defined(ARCH_rpi2)
	#include <tupai/arm/rpi2/arch.hpp>
#else
	#warning "Architecture provides no architecture information!"
#endif

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/arch.hpp>
#elif defined(ARCH_FAMILY_arm)
	#include <tupai/arm/arch.hpp>
#else
	#warning "Architecture provides no family information!"
#endif

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	const char* sys_get_name_decorative()
	{
		return P_NAME_DECORATIVE;
	}

	const char* sys_get_arch()
	{
		#if defined(ARCH_amd64)
			return amd64_get_arch();
		#elif defined(ARCH_i386)
			return i386_get_arch();
		#elif defined(ARCH_rpi2)
			return rpi2_get_arch();
		#else
			return "unknown";
		#endif
	}

	const char* sys_get_family()
	{
		#if defined(ARCH_FAMILY_x86)
			return x86_get_family();
		#elif defined(ARCH_FAMILY_arm)
			return arm_get_family();
		#else
			return "unknown";
		#endif
	}

	const char* sys_get_version()
	{
		return P_VERSION_DECORATIVE;
	}
}
