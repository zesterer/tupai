//
// file : node.hpp
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

#ifndef TUPAI_FS_NODE_HPP
#define TUPAI_FS_NODE_HPP

// Tupai
#include <tupai/fs/com.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace fs
	{
		enum class node_type : uint8_t
		{
			FILE,
			DIRECTORY,
		};

		struct node_t
		{
			id_t      id;
			char*     name;
			node_type type;

			node_t* parent;
			node_t* next;
			node_t* child;

			node_t(node_t* parent, node_type type, const char* name = nullptr);
			~node_t();

			void set_name(const char* name);
			const char* get_name();
			void add_child(node_t* child);
		};
	}
}

#endif
