//
// file : tmpfs.c
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

#include <tupai/fs/tmpfs.h>
#include <tupai/vfs/vfs.h>
#include <tupai/vfs/inode.h>
#include <tupai/util/mem.h>
#include <tupai/util/log.h>

#include <tupai/mem/kheap.h>

typedef struct tmpfs_file
{
	uint8_t* data;
	size_t size;
} tmpfs_file_t;

static void fs_display(inode_t* inode);

int tmpfs_create(fs_t* fs)
{
	int val = vfs_fs_create(fs, "tmpfs");
	if (val)
		return val;
	else
	{
		// Set up function pointers
		fs->display = &fs_display;

		log("[ OK ] Created tmpfs\n");
		return 0;
	}
}

int tmpfs_add(fs_t* fs, uint8_t* data, size_t size)
{
	inode_t* inode = ALLOC_OBJ(inode_t);
	int val = vfs_inode_create(inode, fs);

	tmpfs_file_t* nfile = ALLOC_OBJ(tmpfs_file_t);
	nfile->data = data;
	nfile->size = size;

	inode->internal = nfile;

	return val;
}

void fs_display(inode_t* inode)
{
	tmpfs_file_t* file = inode->internal;
	if (file == nullptr)
		return;
	else
		logf("Inode %u has data at %p of size %u\n", inode->id, (void*)file->data, file->size);
}
