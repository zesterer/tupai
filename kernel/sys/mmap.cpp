//
// file : mmap.cpp
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
#include <tupai/sys/mmap.hpp>
#include <tupai/arch.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace sys
	{
		static const int MMAP_BRANCH_P2 = 10;
		static const int MMAP_BRANCHES = 1 << MMAP_BRANCH_P2;

		static const int MMAP_BR_P2 = 10;
		static const int MMAP_BR    = 1 << MMAP_BR_P2;

		struct mmap_br_t;

		struct mmap_node_t
		{
			pid_t owner = NO_PROC_ID;

			mmap_br_t* br = nullptr;

			~mmap_node_t();
		};

		struct mmap_br_t
		{
			int depth = 0;

			mmap_node_t nodes[MMAP_BR];
		};

		mmap_node_t::~mmap_node_t()
		{
			delete this->br;
		}

		mmap_node_t root;

		void mmap_init()
		{
			root.br = new mmap_br_t();
			root.br->depth = 1;
		}

		void __mmap_reserve(mmap_node_t* node, int depth, size_t addr, pid_t owner, size_t start, size_t size)
		{
			if (addr >= start && addr < start + size)
				node->owner = owner;

			if (node->br != nullptr)
			{
				size_t node_size = (1 << (depth * MMAP_BR_P2));

				if (addr >= start && addr + node_size <= start + size)
				{
					delete node->br;
					return;
				}
			}
			else if (depth > 0)
			{
				size_t node_size = (1 << (depth * MMAP_BR_P2));

				if (start <= addr + node_size && addr <= start + size)
				{
					node->br = new mmap_br_t();
					node->br->depth = depth - 1;
				}
			}

			if (node->br != nullptr)
			{
				for (int i = 0; i < MMAP_BR; i ++)
				{
					__mmap_reserve(&node->br->nodes[i], depth - 1, addr, owner, start, size);
					addr += 1 << (node->br->depth * MMAP_BR_P2);
				}
			}
		}

		void mmap_reserve(size_t start, size_t size, pid_t owner)
		{
			size_t pstart = start / ARCH_PAGE_SIZE;
			size_t psize = util::align_ceiling(start + size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE - pstart;

			__mmap_reserve(&root, root.br->depth + 1, 0, owner, pstart, psize);
		}

		pid_t __mmap_display(mmap_node_t* node, size_t addr, pid_t cowner)
		{
			if (node->br != nullptr)
			{
				for (int i = 0; i < MMAP_BR; i ++)
				{
					cowner = __mmap_display(&node->br->nodes[i], addr, cowner);
					addr += 1 << (node->br->depth * MMAP_BR_P2);
				}

				return cowner;
			}
			else
			{
				if (node->owner != cowner)
				{
					cowner = node->owner;
					util::println(util::fmt_int<size_t>(addr * ARCH_PAGE_SIZE, 16, sizeof(size_t) * 2), " : owner = ", proc_get_name(cowner), " (", cowner, ')');
				}

				return cowner;
			}
		}

		void mmap_display()
		{
			util::println("--- Physical Memory Map (1 page = ", util::fmt_int<uint16_t>(ARCH_PAGE_SIZE, 16), ") ---");
			__mmap_display(&root, 0, -2);
		}
	}
}
