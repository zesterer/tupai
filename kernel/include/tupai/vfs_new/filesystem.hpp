//
// file : filesystem.hpp
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

#ifndef TUPAI_VFS_NEW_FILESYSTEM_HPP
#define TUPAI_VFS_NEW_FILESYSTEM_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/vfs_new/inode.hpp>
#include <tupai/vfs_new/vtable.hpp>

#include <tupai/util/ref.hpp>
#include <tupai/util/vec.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/val.hpp>

namespace tupai
{
	namespace vfs_new
	{
		class Filesystem
		{
		private:
			id_t _id;

			util::Str _name;

			util::WRef<VTable> _vtable;

			util::Ref<Inode> _root;
			util::Vec<util::WRef<Inode>> _inodes;

		public:
			Filesystem(util::Str name);

			util::Str getName() { return this->_name; }
			util::Vec<util::WRef<Inode>>& getInodes() { return this->_inodes; }

			util::Ref<Inode> createInode(Inode::Type type);
		};
	}
}

#endif
