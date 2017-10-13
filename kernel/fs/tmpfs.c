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
#include <tupai/vfs/fs.h>
#include <tupai/vfs/inode.h>
#include <tupai/util/mem.h>
#include <tupai/util/log.h>

#include <tupai/mem/kheap.h>

typedef struct tmpfs_file
{
	uint8_t* data;
	size_t size;
} tmpfs_file_t;

static int fs_inode_create(inode_t* inode);
static int fs_inode_set_raw(inode_t* inode, uint8_t* data, size_t size);
static size_t fs_inode_size(inode_t* inode);
static int fs_inode_display(inode_t* inode);

vtable_t tmpfs_vtable;

int tmpfs_create(fs_t* fs)
{
	// Set up function pointers
	tmpfs_vtable.inode_create = &fs_inode_create;
	tmpfs_vtable.inode_set_raw = &fs_inode_set_raw;
	tmpfs_vtable.inode_size = &fs_inode_size;
	tmpfs_vtable.inode_display = &fs_inode_display;

	// Execute filesystem-independent creation
	int val = vfs_fs_create(fs, &tmpfs_vtable, "tmpfs");
	if (val)
		return val;
	else
	{
		log("[ OK ] Created tmpfs\n");
		return 0;
	}
}

int fs_inode_create(inode_t* inode)
{
	if (inode->type == INODE_NORMAL)
	{
		tmpfs_file_t* nfile = ALLOC_OBJ(tmpfs_file_t);
		nfile->data = nullptr;
		nfile->size = 0;

		inode->internal = nfile;
	}
	else
		inode->internal = nullptr;

	return FS_PROPAGATE;
}

int fs_inode_set_raw(inode_t* inode, uint8_t* data, size_t size)
{
	if (inode->type != INODE_NORMAL)
		return 1;
	else if (inode->internal == nullptr)
		return 2;

	tmpfs_file_t* file = inode->internal;
	file->data = data;
	file->size = size;

	return FS_PROPAGATE;
}

size_t fs_inode_size(inode_t* inode)
{
	if (inode->type != INODE_NORMAL || inode->internal == nullptr)
		return 0;

	tmpfs_file_t* file = inode->internal;

	return file->size;
}

int fs_inode_display(inode_t* inode)
{
	tmpfs_file_t* file = inode->internal;
	if (file == nullptr)
		logf("Inode %u of type %u has no data\n", inode->id, inode->type);
	else
		logf("Inode %u of type %u has data at %p of size %u\n", inode->id, inode->type, (void*)file->data, file->size);

	return FS_TERMINATE;
}
