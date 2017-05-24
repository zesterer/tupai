//
// file : fs.cpp
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
#include <tupai/fs/fs.hpp>
#include <tupai/fs/node.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		node_t root(nullptr);

		static void fs_display_node(node_t* node, size_t depth = 0);

		void fs_init()
		{
			// Root
			auto bin = new node_t(&root, "bin");
			root.add_child(bin);
			auto dev = new node_t(&root, "dev");
			root.add_child(dev);
			auto etc = new node_t(&root, "etc");
			root.add_child(etc);
			auto proc = new node_t(&root, "proc");
			root.add_child(proc);

			// Dev
			auto tty0 = new node_t(dev, "tty0");
			dev->add_child(tty0);
			auto com1 = new node_t(dev, "com1");
			dev->add_child(com1);
		}

		void fs_display()
		{
			util::println("Filesystem:");
			fs_display_node(&root, 1);
		}

		void fs_display_node(node_t* node, size_t depth)
		{
			// Indentation
			for (size_t i = 0; i < depth; i ++)
				util::print("  ");
			// Name
			util::print("", node->get_name(), "/\n");

			node_t* cnode = node->child;
			while (cnode != nullptr)
			{
				fs_display_node(cnode, depth + 1);
				cnode = cnode->next;
			}
		}
	}
}
