//
// file : vfs.cpp
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
#include <tupai/fs/vfs.hpp>
#include <tupai/fs/fs.hpp>
#include <tupai/fs/node.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		util::mutex fs_mutex;
		node_t root(nullptr, node_type::DIRECTORY);
		util::vector_t<fs_dev_t> active_fs_dev;

		static void vfs_display_node(node_t* node, size_t depth = 0);

		void vfs_init()
		{
			fs_mutex.lock(); // Begin critical section

			// Create root
			root = node_t(nullptr, node_type::DIRECTORY);

			// Root
			auto bin = new node_t(&root, node_type::DIRECTORY, "bin");
			root.add_child(bin);
			auto dev = new node_t(&root, node_type::DIRECTORY, "dev");
			root.add_child(dev);
			auto etc = new node_t(&root, node_type::DIRECTORY, "etc");
			root.add_child(etc);
			auto proc = new node_t(&root, node_type::DIRECTORY, "proc");
			root.add_child(proc);

			// Dev
			auto tty0 = new node_t(dev, node_type::FILE, "tty0");
			dev->add_child(tty0);
			auto com1 = new node_t(dev, node_type::FILE, "com1");
			dev->add_child(com1);

			fs_mutex.unlock(); // End critical section
		}

		void vfs_display()
		{
			fs_mutex.lock(); // Begin critical section

			util::println("Filesystem:");
			vfs_display_node(&root, 1);

			fs_mutex.unlock(); // End critical section
		}

		void vfs_display_node(node_t* node, size_t depth)
		{
			// Indentation
			for (size_t i = 0; i < depth; i ++)
				util::print("  ");
			// Name
			util::print("", node->get_name(), (node->type == node_type::DIRECTORY) ? "/" : "", '\n');

			node_t* cnode = node->child;
			while (cnode != nullptr)
			{
				vfs_display_node(cnode, depth + 1);
				cnode = cnode->next;
			}
		}
	}
}
