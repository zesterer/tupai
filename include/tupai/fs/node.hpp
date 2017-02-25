/*
* 	file : node.hpp
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

#ifndef TUPAI_FS_NODE_HPP
#define TUPAI_FS_NODE_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/fs/common.hpp>

#include <tupai/util/string.hpp>

namespace tupai
{
	namespace fs
	{
		enum class node_type : byte
		{
			PLACEHOLDER = 0,
			DIR   = 1,
			FILE  = 2,
			LINK  = 3,
			BLOCK = 4,
		};

		struct node
		{
			util::string name;
			node_type type = node_type::DIR;

			node* parent = nullptr;
			node* next = nullptr;
			node* child = nullptr;

			node() {}
			node(util::string name, node_type type = node_type::PLACEHOLDER, node* parent = nullptr)
			{
				this->name = name;
				this->type = type;
				this->parent = parent;
			}

			status_t add_child(node* nnode)
			{
				if (this->type != node_type::DIR)
					return 1;

				if (this->child == nullptr)
					this->child = nnode;
				else
				{
					node* tmpnode = this->child;

					while (tmpnode->next != nullptr)
					{
						if (tmpnode->name == nnode->name)
							return 1;
						tmpnode = tmpnode->next;
					}

					tmpnode->next = nnode;
				}

				nnode->parent = this;

				return 0;
			}

			node* get_child(util::string name)
			{
				if (this->child == nullptr)
					return nullptr;
				else
				{
					node* tmpnode = this->child;

					while (tmpnode != nullptr)
					{
						if (tmpnode->name == name)
							return tmpnode;
						tmpnode = tmpnode->next;
					}
				}

				return nullptr;
			}

			bool is_root()
			{
				return this->parent == nullptr;
			}

			bool is_directory()
			{
				return this->type == node_type::DIR;
			}

			void get_path(char* buffer)
			{
				if (!this->is_root())
				{
					this->parent->get_path(buffer);

					if (!this->parent->is_root())
						util::cstr_append("/", buffer);

					util::cstr_append(this->name.raw(), buffer);
				}
				else
					util::cstr_copy("/", buffer);
			}
		};
	}
}

#endif
