//
// file : arch.cpp
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
#include <tupai/arch.hpp>
#include <tupai/util/mem.hpp>

namespace tupai
{
	size_t kernel_end_offset = 0;

	size_t arch_get_kernel_start()
	{
		return (size_t)kernel_start;
	}

	size_t arch_get_kernel_end()
	{
		return (size_t)kernel_end + kernel_end_offset;
	}
}
