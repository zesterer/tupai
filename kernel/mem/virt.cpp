//
// file : virt.cpp
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
#include <tupai/mem/virt.hpp>
#include <tupai/mem/kmem.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/log.hpp>
#include <tupai/arch.hpp>

namespace tupai
{
	namespace mem
	{
		namespace virt
		{
			space_t::space_t()
			{
				// TODO : Remove this and make it multi-arch
				this->table = kmem::alloc(1024 * sizeof(void*), ARCH_PAGE_SIZE);
			}
		}
	}
}
