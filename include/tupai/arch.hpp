//
// file : arch.hpp
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

#ifndef TUPAI_ARCH_HPP
#define TUPAI_ARCH_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	const char* arch_get_target();
	const char* arch_get_family();
	size_t arch_get_offset();

	const size_t ARCH_PAGE_SIZE = 4096;

	extern "C" char kernel_start;
	extern "C" char kernel_end;
}

#endif
