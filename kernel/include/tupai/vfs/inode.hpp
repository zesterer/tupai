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

#ifndef TUPAI_VFS_INODE_HPP
#define TUPAI_VFS_INODE_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/util/vector.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace vfs
	{
		enum class inode_mode : uint8_t
		{
			READ    = (1 << 0),
			WRITE   = (1 << 1),
			EXECUTE = (1 << 2),
		};

		struct inode_mode_t
		{
			uint8_t owner_flags = 0xFF; // Allow all by default
			uint8_t group_flags = 0xFF; // Allow all by default
			uint8_t other_flags = 0xFF; // Allow all by default
		};

		struct inode_child_t
		{
			inode_ptr_t inode;
			char name[FILENAME_MAX] = { '\0', };
		};

		struct inode_t
		{
			id_t id = ID_INVALID;
			id_t fs = ID_INVALID;

			util::vector_t<inode_child_t> children;

			inode_type type;
			vtable_t*  vtable = nullptr;

			id_t         owner = ID_INVALID;
			id_t         group = ID_INVALID;
			inode_mode_t mode;

			uint64_t last_access = 0;
			uint64_t last_modify = 0;

			size_t refc = 0;
		};
	}
}

#endif
