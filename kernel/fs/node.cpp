//
// file : node.cpp
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
#include <tupai/fs/node.hpp>
#include <tupai/sys/kmem.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/math.hpp>

namespace tupai
{
	namespace fs
	{
		static const size_t NODE_NAME_MAX_LEN = 255;

		id_t id_counter = 0;
		static id_t node_gen_inode_id();

		node_t::node_t(node_t* parent, node_type type, const char* name)
		{
			this->id = node_gen_inode_id();
			this->name = nullptr;
			this->set_name(name);
			this->type = type;

			this->parent = parent;
			this->next   = nullptr;
			this->child  = nullptr;
		}

		node_t::~node_t()
		{
			if (this->name != nullptr)
				sys::kmem_dealloc(this->name);

			// Recursively delete children
			node_t* cnode = this->child;
			while (cnode != nullptr)
			{
				node_t* onode = cnode;
				cnode = cnode->next;
				delete onode;
			}
		}

		void node_t::set_name(const char* name)
		{
			if (this->name != nullptr)
				sys::kmem_dealloc(this->name);

			if (name == nullptr)
				this->name = nullptr;
			else
			{
				size_t new_len = util::min(NODE_NAME_MAX_LEN, util::str_len(name));
				this->name = (char*)sys::kmem_alloc(sizeof(char) * (new_len + 1));
				util::str_cpy_n(name, this->name, new_len + 1);
			}
		}

		const char* node_t::get_name()
		{
			if (this->name == nullptr)
				return "";
			else
				return this->name;
		}

		void node_t::add_child(node_t* child)
		{
			if (this->child == nullptr)
				this->child = child;
			else
			{
				node_t* cchild = this->child;
				while (cchild->next != nullptr)
					cchild = cchild->next;

				cchild->next = child;
			}
		}

		id_t node_gen_inode_id()
		{
			return id_counter++;
		}
	}
}
