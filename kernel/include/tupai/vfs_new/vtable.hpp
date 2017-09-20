//
// file : vtable.hpp
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

#ifndef TUPAI_VFS_NEW_VTABLE_HPP
#define TUPAI_VFS_NEW_VTABLE_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/vfs_new/inode.hpp>

#include <tupai/util/ref.hpp>
#include <tupai/util/result.hpp>
#include <tupai/util/str.hpp>

namespace tupai
{
	namespace vfs_new
	{
		class Inode;

		class VTable
		{
			// int      (*open) (inode_ptr_t inode)                           = nullptr;
			// int      (*close)(fd_ptr_t desc)                               = nullptr;
			// ssize_t  (*read) (fd_ptr_t desc, void* rbuff, size_t n)        = nullptr;
			// ssize_t  (*write)(fd_ptr_t desc, const void* buff, size_t n)   = nullptr;
			// int      (*seek) (fd_ptr_t desc, int origin, fd_offset offset) = nullptr;

			util::Status (*open)(util::WRef<Inode> inode) = nullptr;
		};
	}
}

#endif