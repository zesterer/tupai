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

#ifndef TUPAI_VFS_NEW_VFS_HPP
#define TUPAI_VFS_NEW_VFS_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/util/ref.hpp>
#include <tupai/util/result.hpp>
#include <tupai/util/str.hpp>

#include <tupai/vfs_new/filesystem.hpp>

namespace tupai
{
	namespace vfs_new
	{
		void init();
		void display();

		util::Ref<Filesystem> createFilesystem(util::Str name);
	}
}

#endif