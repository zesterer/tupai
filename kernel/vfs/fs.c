//
// file : fs.c
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

#include <tupai/vfs/fs.h>
#include <tupai/util/path.h>

/*
inode_t* fs_get_rel_inode(fs_t* fs, const char* path)
{
	size_t nelem = path_elements(path);

	inode_t* cnode = fs->root;
	for (size_t i = 0; i < nelem && cnode != nullptr; i ++)
	{
		char elem[ELEMENT_MAX_LEN + 1];
		path_extract(path, i, elem);
		cnode = inode_get_child(cnode, elem);
	}

	if (cnode == nullptr)
		return nullptr;
	else
		return cnode;
}

inode_t* fs_create_path(fs_t* fs, const char* path)
{
	size_t nelem = path_elements(path);

	inode_t* cnode = fs->root;
	for (size_t i = 0; i < nelem; i ++)
	{
		char elem[ELEMENT_MAX_LEN + 1];
		path_extract(path, i, elem)

		inode_t* child = inode_get_child(cnode, elem);
		if (child == nullptr)
			inode_c
	}
}
*/
