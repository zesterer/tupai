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
		return (size_t)&kernel_start;
	}

	size_t arch_get_kernel_end()
	{
		return (size_t)&kernel_end + kernel_end_offset;
	}

	void* arch_kernel_alloc(size_t bytes)
	{
		// Make sure the kernel end is aligned with pages
		size_t end = util::align_ceiling((size_t)&kernel_end + kernel_end_offset, ARCH_PAGE_SIZE);
		kernel_end_offset += end - (size_t)&kernel_end;

		// Round up the number of required bytes to the nearest page
		bytes = util::align_ceiling(bytes, ARCH_PAGE_SIZE);

		size_t old_end = (size_t)&kernel_end + kernel_end_offset;
		kernel_end_offset += bytes; // Increment kernel end by allocated byte count

		return (void*)(old_end + arch_get_offset());
	}
}
