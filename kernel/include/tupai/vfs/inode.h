//
// file : inode.h
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

#ifndef TUPAI_VFS_INODE_H
#define TUPAI_VFS_INODE_H

#include <tupai/util/strtable.h>
#include <tupai/type.h>

enum
{
	INODE_NORMAL    = 0,
	INODE_DIRECTORY = 1,
	INODE_FIFO      = 2,
	INODE_SOCKET    = 3,
};

typedef struct fs fs_t;
typedef struct inode inode_t;

typedef struct inode_child
{
	char* name;
	inode_t* inode;
} inode_child_t;

typedef struct inode
{
	id_t id;

	int type;
	fs_t* fs;

	size_t ref;
	strtable_t children;

	void* internal;
} inode_t;

inode_t* inode_get_child(inode_t* inode, const char* filename);

#endif
