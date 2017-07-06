//
// file : desc.cpp
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
#include <tupai/fs/desc.hpp>
#include <tupai/fs/vfs.hpp>
#include <tupai/fs/vtable.hpp>

#include <tupai/sys/proc.hpp>

#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		id_t desc_open(inode_t* inode)
		{
			id_t cproc  = sys::proc_get_current();

			if (inode->vtable == nullptr)
				return 0;
			if (inode->vtable->open == nullptr)
				return 0;

			return inode->vtable->open(cproc, inode);
		}

		int desc_close(id_t desc)
		{
			id_t     cproc = sys::proc_get_current();
			desc_t*  cdesc = sys::proc_get_desc(cproc, desc);

			if (cdesc == nullptr)
				return -1;

			inode_t* cinode = vfs_get_inode(cdesc->inode);

			if (cinode == nullptr)
				return -1;
			if (cinode->vtable == nullptr)
				return -1;
			if (cinode->vtable->close == nullptr)
				return -1;

			return cinode->vtable->close(cproc, cdesc);
		}

		ssize_t desc_read(id_t desc, size_t n, void* ret_buff)
		{
			id_t     cproc = sys::proc_get_current();
			desc_t*  cdesc = sys::proc_get_desc(cproc, desc);

			if (cdesc == nullptr)
				return -1;

			inode_t* cinode = vfs_get_inode(cdesc->inode);

			if (cinode == nullptr)
				return -1;
			if (cinode->vtable == nullptr)
				return -1;
			if (cinode->vtable->read == nullptr)
				return -1;

			return cinode->vtable->read(cproc, cdesc, n, ret_buff);
		}
	}
}
