//
// file : fs.h
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

#ifndef TUPAI_VFS_FS_H
#define TUPAI_VFS_FS_H

#include <tupai/vfs/inode.h>
#include <tupai/vfs/vtable.h>
#include <tupai/type.h>

enum
{
	FS_PROPAGATE = 0,
	FS_TERMINATE = 1,
};

typedef struct fs
{
	id_t id;
	char* name;

	inode_t* root;
} fs_t;

inode_t* fs_get_rel_inode(fs_t* fs, const char* path);
inode_t* fs_create_path(fs_t* fs, const char* path);

#endif