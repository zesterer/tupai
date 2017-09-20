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
#include <tupai/vfs_new/vfs.hpp>
#include <tupai/vfs_new/filesystem.hpp>
#include <tupai/vfs_new/inode.hpp>
#include <tupai/vfs_new/vtable.hpp>

namespace tupai
{
	namespace vfs_new
	{
		util::Vec<util::WRef<Filesystem>> fs_track;
		util::WRef<Inode> root;

		void init()
		{
			util::logln("Started VFS...");
		}

		void display()
		{
			for (size_t i = 0; i < fs_track.size(); i ++)
			{
				util::logln("Filesystem[", i, "] = ", fs_track[i]->getName());
				util::logln("--> Root inode");

				for (size_t i = 0; i < fs_track[i]->getInodes().size(); i ++)
					util::logln("--> Inode");
			}
		}

		util::Ref<Filesystem> createFilesystem(util::Str name)
		{
			util::Ref<Filesystem> nfs(name);
			fs_track.push(~nfs);

			return nfs;
		}
	}
}
