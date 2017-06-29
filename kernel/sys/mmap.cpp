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
			else
				node->owner = NO_PROC_ID;

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
			size_t psize = util::align_ceiling(start + size, ARCH_PAGE_SIZE) / ARCH_PAGE_SIZE;

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
			util::println("--- Memory Map ---");
			__mmap_display(&root, 0, -2);
		}

		/*
		struct mmap_entry_t
		{
			uint8_t depth = 0;
			pid_t   owner = NO_PROC_ID;

			mmap_entry_t** entries = nullptr;
		};

		mmap_entry_t mmap_root;
		mmap_entry_t mmap_root2;

		void mmap_init()
		{
			mmap_root.entries = new mmap_entry_t*[MMAP_BRANCHES];
			for (size_t i = 0; i < MMAP_BRANCHES; i ++)
				mmap_root.entries[i] = nullptr;

			mmap_root.depth = (sizeof(size_t) * 8) - MMAP_BRANCH_P2;
			mmap_root.owner = NO_PROC_ID;

			mmap_root2.depth = (sizeof(size_t) * 8) - MMAP_BRANCH_P2 * 2;
			mmap_root2.entries = new mmap_entry_t*[MMAP_BRANCHES];
			for (size_t i = 0; i < MMAP_BRANCHES; i ++)
				mmap_root2.entries[i] = nullptr;
			mmap_root2.owner = KERNEL_PROC_ID;
			mmap_root2.entries = nullptr;
			mmap_root.entries[7] = &mmap_root2;
			mmap_root.entries[12] = &mmap_root2;
			mmap_root.entries[13] = &mmap_root2;
		}

		void mmap_reserve(size_t start, size_t count)
		{
			pid_t cowner = -2;
			mmap_entry_t* cmap = &mmap_root;

			size_t addr = 0x0;

			while (true)
			{
				size_t offset = (1 << cmap->depth);

				if (addr + offset < start)
				{
					if (cmap->entries == nullptr)
					{
						cmap->entries = new mmap_entry_t*[MMAP_BRANCHES];
						for (size_t i = 0; i < MMAP_BRANCHES; i ++)
							cmap->entries[i] = nullptr;
					}

					i = 0;
				}
			}
		}

		void mmap_display()
		{
			pid_t cowner = -2;
			mmap_entry_t* cmap = &mmap_root;

			mmap_entry_t* mhist[12];
			int bhist[12];
			int hist = 0;

			size_t addr = 0x0;

			int i = 0;
			while (true)
			{
				if (cowner != cmap->owner)
				{
					cowner = cmap->owner;
					util::println(util::fmt_int<size_t>(addr, 16, sizeof(size_t) * 2), " : owner = ", cowner);
				}

				if (cmap->entries != nullptr && i < MMAP_BRANCHES)
				{
					if (cmap->entries[i] != nullptr)
					{
						mhist[hist] = cmap;
						bhist[hist] = i + 1;
						hist ++;

						cmap = cmap->entries[i];
						i = 0;

						continue;
					}
					else
					{
						i ++;
					}
				}

				if (cmap->entries == nullptr || i == MMAP_BRANCHES)
				{
					if (hist == 0)
						break;
					else
					{
						hist --;
						cmap = mhist[hist];
						i = bhist[hist];
						addr += (1 << cmap->depth);
					}
				}
				else
					addr += (1 << cmap->depth);
			}
		}*/
	}
}
