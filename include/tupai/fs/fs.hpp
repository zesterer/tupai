/*
* 	file : fs.hpp
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

#ifndef TUPAI_FS_FS_HPP
#define TUPAI_FS_FS_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/fs/common.hpp>
#include <tupai/fs/node.hpp>

namespace tupai
{
	namespace fs
	{
		void fs_init();

		void fs_print();

		node*    fs_find(const char* path, const char* rel = nullptr);
		status_t fs_list_children(node* parent, node** nodes, umem* n, umem max = 0);
	}
}

#endif
