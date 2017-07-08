//
// file : inode.hpp
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

#ifndef TUPAI_FS_INODE_HPP
#define TUPAI_FS_INODE_HPP

// Tupai
#include <tupai/fs/com.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/table.hpp>
#include <tupai/util/spinlock.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace fs
	{
		struct inode_child_t
		{
			id_t inode;
			char name[FILENAME_SIZE] = "\0";

			void set_name(const char* name);
			const char* get_name();
		};

		struct inode_t
		{
			id_t gid = 0;
			id_t id = 0;
			id_t fs = 0;

			inode_type type = inode_type::NORMAL_FILE;
			vtable_t* vtable = nullptr;

			util::vector_t<inode_child_t> dir_table;

			id_t     owner;
			id_t     group;
			uint16_t mode;

			size_t ref_count = 0;

			uint64_t last_access;
			uint64_t last_modify;

			inode_t() {}
			inode_t(inode_type type)
			{
				this->type = type;
			};
		};

		//void inode_add_child(inode_t* parent, inode_t* child, const char* name);
		//inode_t* inode_get_child(inode_t* parent, const char* name);

		void inode_add_child (id_t id, id_t child, const char* name);
		void inode_set_fs    (id_t id, id_t fs);
		void inode_set_vtable(id_t id, vtable_t* table);

		id_t        inode_get_child     (id_t id, const char* name);
		id_t        inode_get_nth_child (id_t id, size_t n);
		const char* inode_get_child_name(id_t id, id_t child);
		vtable_t*   inode_get_vtable    (id_t id);
		inode_type  inode_get_type      (id_t id);
		size_t      inode_child_count   (id_t id);
	}
}

#endif
