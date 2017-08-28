//
// file : vfs.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

// Tupai
#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/fs.hpp>
#include <tupai/vfs/inode.hpp>
#include <tupai/vfs/fd.hpp>
#include <tupai/vfs/path.hpp>
#include <tupai/util/spinlock.hpp>
#include <tupai/util/hashtable.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/log.hpp>

namespace tupai
{
	namespace vfs
	{
		static util::spinlock_t spinlock;

		static util::hashtable_t<fs_t>    fs_table;
		static util::hashtable_t<inode_t> inode_table;
		static util::hashtable_t<fd_t>    fd_table;

		static id_t fs_counter    = 0;
		static id_t inode_counter = 0;
		static id_t fd_counter    = 0;

		static inode_ptr_t vfs_root = ID_INVALID;

		/* VFS Functions */

		inode_ptr_t get_root()
		{
			spinlock.lock(); // Begin critical section

			inode_ptr_t val = vfs_root;

			spinlock.unlock(); // End critical section
			return val;
		}

		inode_ptr_t get_inode(const char* path)
		{
			spinlock.lock(); // Begin critical section

			size_t element_count = path_element_count(path);
			inode_ptr_t cinode = vfs_root;
			for (size_t i = 0; i < element_count; i ++)
			{
				char element[FILENAME_MAX];
				path_element_get(path, element, i);

				inode_t* inode = inode_table[cinode];
				cinode = ID_INVALID;
				if (inode != nullptr)
				{
					for (size_t i = 0; i < inode->children.size(); i ++)
					{
						if (util::str_equal(inode->children[i].name, element))
						{
							cinode = inode->children[i].inode;
							break;
						}
					}
				}

				if (cinode == ID_INVALID)
					break;
			}

			spinlock.unlock(); // End critical section
			return cinode;
		}

		int set_root(inode_ptr_t root)
		{
			spinlock.lock(); // Begin critical section

			vfs_root = root;

			spinlock.unlock(); // End critical section
			return 0;
		}

		int mount(inode_ptr_t inode, const char* path, bool create)
		{
			return vfs_root.mount_relative(inode, path, create);
		}

		void init()
		{
			// Nothing yet
		}

		void vfs_print_inode(inode_ptr_t inode, const char* name, size_t depth = 0)
		{
			if (inode == ID_INVALID)
				return;

			// Indentation
			for (size_t i = 0; i < depth; i ++)
				util::log((i == depth - 1) ? "|--" : "|  ");

			inode_type type;
			inode.get_type(&type);

			// Name
			util::logln(name, (type == inode_type::DIRECTORY) ? "/" : "");

			for (size_t i = 0; i < inode.get_children(); i ++)
			{
				inode_ptr_t ninode = inode.get_nth_child(i);

				char name[FILENAME_MAX];
				inode.get_child_name(ninode, name);

				vfs_print_inode(ninode, name, depth + 1);
			}
		}

		void display()
		{
			util::logln("--- Virtual Filesystem ---");

			vfs_print_inode(vfs_root, "");

			util::log('\n');

			util::logln("--- Filesystem Devices ---");
			for (size_t i = 0; i < fs_table.size(); i ++)
				util::logln(fs_table.nth(i)->id, " : ", fs_table.nth(i)->name);
		}

		fs_ptr_t create_fs(const char* name)
		{
			// Create a root inode for the filesystem
			inode_ptr_t fsroot = create_inode(inode_type::DIRECTORY, ID_INVALID);

			spinlock.lock(); // Begin critical section

			id_t nid = fs_counter ++; // Generate a new filesystem ID

			fs_t nfs;
			nfs.id = nid;
			nfs.root = fsroot;
			util::str_cpy_n(name, nfs.name, FS_NAME_MAX);

			fs_table.add(nid, nfs); // Add the new filesystem to the fs table

			spinlock.unlock(); // End critical section

			fsroot.set_fs(nid);

			return nid;
		}

		inode_ptr_t create_inode(inode_type type, fs_ptr_t fs)
		{
			spinlock.lock(); // Begin critical section

			id_t nid = inode_counter ++; // Generate a new inode ID

			inode_t ninode;
			ninode.id = nid;
			ninode.fs = fs;
			ninode.type = type;
			ninode.vtable = nullptr;
			inode_table.add(nid, ninode); // Add the new inode to the inode table

			spinlock.unlock(); // End critical section
			return nid;
		}

		fd_ptr_t create_fd(inode_ptr_t inode)
		{
			spinlock.lock(); // Begin critical section

			id_t nid = fd_counter ++; // Generate a new file descriptor ID

			fd_t nfd;
			nfd.id = nid;
			nfd.inode = inode;
			fd_table.add(nid, nfd); // Add the new file descriptor to the fd table

			spinlock.unlock(); // End critical section
			return nid;
		}

		int delete_fs(fs_ptr_t fd)
		{
			spinlock.lock(); // Begin critical section

			bool removed = fs_table.remove(fd);

			int err = 1;
			if (removed)
				err = 0;

			spinlock.unlock(); // End critical section
			return err;
		}

		int delete_inode(inode_ptr_t inode)
		{
			spinlock.lock(); // Begin critical section

			bool removed = inode_table.remove(inode);

			int err = 1;
			if (removed)
				err = 0;

			spinlock.unlock(); // End critical section
			return err;
		}

		int delete_fd(fd_ptr_t fd)
		{
			spinlock.lock(); // Begin critical section

			bool removed = fd_table.remove(fd);

			int err = 1;
			if (removed)
				err = 0;

			spinlock.unlock(); // End critical section
			return err;
		}

		/* FS Functions */

		int fs_ptr_t::get_name(char* buff, size_t n)
		{
			spinlock.lock(); // Begin critical section

			fs_t* fs = fs_table[this->id];

			int err = 1;
			if (fs != nullptr)
			{
				util::str_cpy_n(fs->name, buff, n);
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		inode_ptr_t fs_ptr_t::get_root()
		{
			spinlock.lock(); // Begin critical section

			fs_t* fs = fs_table[this->id];

			inode_ptr_t val = ID_INVALID;
			if (fs != nullptr)
				val = fs->root;

			spinlock.unlock(); // End critical section
			return val;
		}

		int fs_ptr_t::set_name(const char* name)
		{
			spinlock.lock(); // Begin critical section

			fs_t* fs = fs_table[this->id];

			int err = 1;
			if (fs != nullptr)
			{
				util::str_cpy_n(name, fs->name, FS_NAME_MAX);
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int fs_ptr_t::set_root(inode_ptr_t root)
		{
			spinlock.lock(); // Begin critical section

			fs_t* fs = fs_table[this->id];

			int err = 1;
			if (fs != nullptr)
			{
				fs->root = root;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		/* Inode Functions */

		id_t inode_ptr_t::get_fs()
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			id_t val = ID_INVALID;
			if (inode != nullptr)
				val = inode->fs;

			spinlock.unlock(); // End critical section
			return val;
		}

		int inode_ptr_t::get_type(inode_type* rtype)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			*rtype = inode_type::NORMAL_FILE;
			if (inode != nullptr)
			{
				*rtype = inode->type;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int inode_ptr_t::get_vtable(vtable_t** rvtable)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			*rvtable = nullptr;
			if (inode != nullptr)
			{
				*rvtable = inode->vtable;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		size_t inode_ptr_t::get_children()
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			size_t val = 0;
			if (inode != nullptr)
				val = inode->children.size();

			spinlock.unlock(); // End critical section
			return val;
		}

		inode_ptr_t inode_ptr_t::get_child(const char* name)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			inode_ptr_t val = ID_INVALID;
			if (inode != nullptr)
			{
				for (size_t i = 0; i < inode->children.size(); i ++)
				{
					if (util::str_equal(inode->children[i].name, name))
					{
						val = inode->children[i].inode;
						break;
					}
				}
			}

			spinlock.unlock(); // End critical section
			return val;
		}

		inode_ptr_t inode_ptr_t::get_nth_child(size_t n)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			inode_ptr_t val = ID_INVALID;
			if (inode != nullptr)
				val = inode->children[n].inode;

			spinlock.unlock(); // End critical section
			return val;
		}

		int inode_ptr_t::get_child_name(inode_ptr_t child, char* rname, size_t n)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			rname[0] = '\0';
			if (inode != nullptr)
			{
				for (size_t i = 0; i < inode->children.size(); i ++)
				{
					if (inode->children[i].inode == child)
					{
						util::str_cpy_n(inode->children[i].name, rname, n);
						err = 0;
						break;
					}
				}
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int inode_ptr_t::set_type(inode_type type)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			if (inode != nullptr)
			{
				inode->type = type;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int inode_ptr_t::set_vtable(vtable_t* vtable)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			if (inode != nullptr)
			{
				inode->vtable = vtable;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int inode_ptr_t::set_fs(fs_ptr_t fs)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			if (inode != nullptr)
			{
				inode->fs = fs;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int inode_ptr_t::mount_child(inode_ptr_t child, const char* name)
		{
			spinlock.lock(); // Begin critical section

			inode_t* inode = inode_table[this->id];

			int err = 1;
			if (inode != nullptr)
			{
				inode_child_t nchild;
				nchild.inode = child;
				util::str_cpy_n(name, nchild.name, FILENAME_MAX);
				inode->children.push(nchild);

				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int inode_ptr_t::mount_relative(inode_ptr_t child, const char* path, bool create)
		{
			spinlock.lock(); // Begin critical section

			int err = 1;

			size_t element_count = path_element_count(path);
			inode_ptr_t cinode = *this;
			for (size_t i = 0; i < element_count - 1; i ++)
			{
				char element[FILENAME_MAX];
				path_element_get(path, element, i);

				inode_t* inode = inode_table[cinode];
				cinode = ID_INVALID;
				if (inode != nullptr)
				{
					for (size_t i = 0; i < inode->children.size(); i ++)
					{
						if (util::str_equal(inode->children[i].name, element))
						{
							cinode = inode->children[i].inode;
							break;
						}
					}
				}

				if (cinode == ID_INVALID)
				{
					if (create)
					{
						inode_ptr_t cparent = (inode_ptr_t)inode->id;

						spinlock.unlock(); // End critical section
						inode_ptr_t ndir = create_inode(inode_type::DIRECTORY, cparent.get_fs());
						cparent.mount_child(ndir, element);
						spinlock.lock(); // Re-begin critical section

						cinode = ndir;
					}
					else
						break;
				}
			}

			if (cinode != ID_INVALID)
			{
				inode_t* dir = inode_table[cinode];
				char filename[FILENAME_MAX];
				path_element_get(path, filename, element_count - 1);

				if (dir != nullptr)
				{
					inode_child_t nchild;
					nchild.inode = child;
					util::str_cpy_n(filename, nchild.name, FILENAME_MAX);
					dir->children.push(nchild);

					err = 0;
				}
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		/* FD Functions */

		inode_ptr_t fd_ptr_t::get_inode()
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			inode_ptr_t val = ID_INVALID;
			if (fd != nullptr)
				val = fd->inode;

			spinlock.unlock(); // End critical section
			return val;
		}

		int fd_ptr_t::get_mode(fd_mode_t* rmode)
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			int err = 1;
			*rmode = fd_mode_t();
			if (fd != nullptr)
			{
				*rmode = fd->mode;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		fd_offset fd_ptr_t::get_offset()
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			fd_offset val = 0;
			if (fd != nullptr)
				val = fd->offset;

			spinlock.unlock(); // End critical section
			return val;
		}

		bool fd_ptr_t::get_flag(fd_flag flag)
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			bool val = false;
			if (fd != nullptr)
				val = (fd->flags & (fd_bitflags)flag) != 0;

			spinlock.unlock(); // End critical section
			return val;
		}

		int fd_ptr_t::set_inode(inode_ptr_t inode)
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			int err = 1;
			if (fd != nullptr)
			{
				fd->inode = inode;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int fd_ptr_t::set_mode(fd_mode_t mode)
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			int err = 1;
			if (fd != nullptr)
			{
				fd->mode = mode;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int fd_ptr_t::set_offset(fd_offset offset)
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			int err = 1;
			if (fd != nullptr)
			{
				fd->offset = offset;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int fd_ptr_t::set_flag(fd_flag flag, bool enabled)
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			int err = 1;
			if (fd != nullptr)
			{
				if (enabled)
					fd->flags |= (fd_bitflags)flag;
				else
					fd->flags &= ~(fd_bitflags)flag;
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}

		int fd_ptr_t::destroy()
		{
			spinlock.lock(); // Begin critical section

			fd_t* fd = fd_table[this->id];

			int err = 1;
			if (fd != nullptr)
			{
				fd_table.remove(this->id);
				err = 0;
			}

			spinlock.unlock(); // End critical section
			return err;
		}
	}
}
