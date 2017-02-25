/*
* 	file : fs.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/fs/fs.hpp>
#include <tupai/fs/node.hpp>

#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		node root_node;

		void fs_init()
		{
			root_node = node("root", node_type::DIR);
			root_node.add_child(new node("bin", node_type::DIR));
			root_node.add_child(new node("conf", node_type::DIR));
			root_node.add_child(new node("data", node_type::DIR));
			root_node.add_child(new node("dev", node_type::DIR));
			root_node.add_child(new node("include", node_type::DIR));
			root_node.add_child(new node("lib", node_type::DIR));
			root_node.add_child(new node("media", node_type::DIR));
			root_node.add_child(new node("proc", node_type::DIR));
			root_node.add_child(new node("sys", node_type::DIR));

			root_node.get_child("dev")->add_child(new node("vga", node_type::BLOCK));

			root_node.get_child("sys")->add_child(new node("http", node_type::DIR));
			root_node.get_child("sys")->get_child("http")->add_child(new node("com", node_type::DIR));

			fs_find("/dev")->add_child(new node("console", node_type::FILE));
		}

		node* fs_find(const char* path, const char* rel)
		{
			char pathbuff[512];
			if (rel == nullptr)
				pathbuff[0] = '\0';
			else
				util::cstr_copy(rel, pathbuff);
			util::cstr_append(path, pathbuff);

			if (pathbuff[0] == '/')
			{
				node* cnode = &root_node;

				umem start = 1;
				umem end = start;
				umem len = util::cstr_length(pathbuff);

				while (start < len)
				{
					while (pathbuff[end] != '/' && pathbuff[end] != '\0')
						end ++;

					char name_buff[node_name::MAX_LENGTH + 1];
					util::cstr_copy(pathbuff + sizeof(char) * start, name_buff, util::min(node_name::MAX_LENGTH, end - start));

					if (util::cstr_equal(name_buff, "."))
						cnode = cnode;
					else if (util::cstr_equal(name_buff, ".."))
						cnode = cnode->parent;
					else
						cnode = cnode->get_child(name_buff);

					if (cnode == nullptr)
						return nullptr;

					end ++;
					start = end;
				}

				return cnode;
			}
			else
				return nullptr;
		}

		status_t fs_list_children(node* parent, node** nodes, umem* n, umem max)
		{
			if (parent == nullptr)
				return 1;

			if (parent->child == nullptr)
			{
				*n = 0;
				return 0;
			}
			else
			{
				node* tmpnode = parent->child;
				nodes[0] = tmpnode;
				umem count = 1;

				while (tmpnode != nullptr && (count < max || max == 0))
				{
					tmpnode = tmpnode->next;
					nodes[count] = tmpnode;
					count ++;
				}

				*n = count - 1;

				return 0;
			}
		}
	}
}
