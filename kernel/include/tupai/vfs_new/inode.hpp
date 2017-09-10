//
// file : inode.hpp
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

#ifndef TUPAI_VFS_NEW_INODE_HPP
#define TUPAI_VFS_NEW_INODE_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/vfs_new/filesystem.hpp>

#include <tupai/util/ref.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/string.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace vfs_new
	{
		class Inode;
		class Filesystem;

		class InodeChild
		{
			util::Ref<Inode> inode;
			util::String name;

			InodeChild(util::Ref<Inode> inode, const util::String& name) : inode(inode)
			{
				this->name = name;
			}
		};

		class Inode
		{
		public:
			enum class Mode : uint8_t
			{
				READ  = (1 << 0),
				WRITE = (1 << 0),
				EXEC  = (1 << 0),
			};

			enum class Type
			{
				NORMAL    = 0,
				SYMBOLIC  = 1,
				CHARDEV   = 2,
				BLOCKDEV  = 3,
				DIRECTORY = 4,
				FIFO      = 5,
				UNKNOWN   = 6,
			};

			id_t id;
			util::Ref<Filesystem> fs;

			util::Vector<InodeChild> children;

			Type type;

			uint8_t mode_owner;
			uint8_t mode_group;
			uint8_t mode_other;

			//util::WRef<User>  owner; // TODO : Uncomment this!
			//util::WRef<Group> group; // TODO : Uncomment this!

			uint64_t last_access;
			uint64_t last_modify;
		};
	}
}

#endif
