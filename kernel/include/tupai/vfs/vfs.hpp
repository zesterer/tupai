//
// file : vfs.hpp
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

#ifndef TUPAI_VFS_VFS_HPP
#define TUPAI_VFS_VFS_HPP

// Tupai
#include <tupai/type.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace vfs
	{
		struct fs_ptr_t;
		struct inode_ptr_t;
		struct fd_mode_t;
		struct fd_ptr_t;

		struct vtable_t;

		typedef uint64_t fd_offset;

		struct fs_ptr_t
		{
			id_t id = ID_INVALID;

			fs_ptr_t() {}
			fs_ptr_t(id_t id) { this->id = id; }
			operator id_t() { return this->id; }

			int         get_name(char* buff, size_t n);
			inode_ptr_t get_root();

			int set_name(const char* name);
			int set_root(inode_ptr_t root);
		};

		enum class inode_type
		{
			NORMAL_FILE  = 0,
			HARD_LINK    = 1,
			SYM_LINK     = 2,
			CHAR_DEVICE  = 3,
			BLOCK_DEVICE = 4,
			DIRECTORY    = 5,
			PIPE         = 6,
		};

		const size_t FILENAME_MAX = 256;

		struct inode_ptr_t
		{
			id_t id = ID_INVALID;

			inode_ptr_t() {}
			inode_ptr_t(id_t id) { this->id = id; }
			operator id_t() { return this->id; }

			template <size_t SIZE>
			int get_child_name(inode_ptr_t child, char(&rname)[SIZE]) { return this->get_child_name(child, rname, SIZE); }

			int         get_type      (inode_type* rtype);
			int         get_vtable    (vtable_t** rvtable);
			size_t      get_children  ();
			inode_ptr_t get_child     (const char* name);
			inode_ptr_t get_nth_child (size_t n);
			int         get_child_name(inode_ptr_t child, char* rname, size_t n);

			int set_type   (inode_type type);
			int set_vtable (vtable_t* vtable);
			int set_fs     (fs_ptr_t fs);
			int mount_child(inode_ptr_t child, const char* name);
		};

		enum class fd_mode : uint8_t
		{
			READ  = (1 << 0),
			WRITE = (1 << 1),
		};

		struct fd_mode_t
		{
			uint8_t flags = 0xFF; // All enabled by default
		};

		enum class fd_flag
		{
			EOF = (1 << 0), // End Of File
		};

		typedef uint16_t fd_bitflags;

		struct fd_ptr_t
		{
			id_t id = ID_INVALID;

			fd_ptr_t() {}
			fd_ptr_t(id_t id) { this->id = id; }
			operator id_t() { return this->id; }

			inode_ptr_t get_inode ();
			int         get_mode  (fd_mode_t* rmode);
			fd_offset   get_offset();
			bool        get_flag(fd_flag flag);

			int set_inode (inode_ptr_t inode);
			int set_mode  (fd_mode_t mode);
			int set_offset(fd_offset offset);
			int set_flag(fd_flag flag, bool enabled = true);

			int destroy();
		};

		inode_ptr_t get_root();
		inode_ptr_t get_inode(const char* path);
		int         set_root(inode_ptr_t root);
		int         mount(inode_ptr_t inode, const char* path);

		void init();
		void display();

		fs_ptr_t    create_fs(const char* name);
		inode_ptr_t create_inode(inode_type type);
		fd_ptr_t    create_fd(inode_ptr_t inode);

		int delete_fs(fs_ptr_t fd);
		int delete_inode(inode_ptr_t inode);
		int delete_fd(fd_ptr_t fd);
	}
}

#endif
