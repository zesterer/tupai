//
// file : sys.c
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
#include <tupai/common/sys.h>

#if defined(ARCH_x86_64)
	#include <tupai/x86_family/x86_64/arch.h>
#elif defined(ARCH_i386)
	#include <tupai/x86_family/i386/arch.h>
#elif defined(ARCH_arm)
	#include <tupai/arm/arch.h>
#else
	#warning "Architecture provides no architecture information!"
#endif

// Standard
#include <stddef.h>
#include <stdint.h>

const char* sys_get_name_decorative()
{
	return P_NAME_DECORATIVE;
}

const char* sys_get_arch()
{
	#if defined(ARCH_x86_64)
		return x86_64_get_arch();
	#elif defined(ARCH_i386)
		return i386_get_arch();
	#elif defined(ARCH_arm)
		return arm_get_arch();
	#else
		return "unknown";
	#endif
}

const char* sys_get_version()
{
	return P_VERSION_DECORATIVE;
}
