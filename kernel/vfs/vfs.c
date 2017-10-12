//
// file : vfs.c
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

#include <tupai/vfs/vfs.h>
#include <tupai/util/mem.h>
#include <tupai/util/log.h>
#include <tupai/util/str.h>
#include <tupai/util/table.h>
#include <tupai/util/panic.h>

table_t fs_table;
id_t fs_count = 0;

table_t inode_table;
id_t inode_count = 0;

fs_t* rootfs = nullptr;

void vfs_init()
{
	if (table_create(&fs_table))
		panic("Unable to initiate VFS filesystem table\n");
	else if (table_create(&inode_table))
		panic("Unable to initiate VFS inode table\n");
	else
		log("[ OK ] Initiated VFS\n");
}

int vfs_fs_create(fs_t* fs, const char* name)
{
	id_t nid = ++fs_count;

	table_add(&fs_table, nid, fs); // Add it to the filesystem table

	fs->id = nid;
	fs->name = str_new(name); // Copy the name across

	fs->root = ALLOC_OBJ(inode_t); // Create a root directory inode
	vfs_inode_create(fs->root, fs, INODE_DIRECTORY);

	// If no root filesystem exists yet, set this one as root
	if (rootfs == nullptr)
	{
		rootfs = fs;
		logf("[ OK ] Root filesystem set to %u (%s)\n", fs->id, fs->name);
	}

	return 0;
}

void vfs_fs_delete(fs_t* fs)
{
	// Delete / dealloc attributes
	dealloc(fs->name);
	vfs_inode_delete(fs->root);

	dealloc(fs);
}

int vfs_inode_create(inode_t* inode, fs_t* fs, int type)
{
	id_t nid = ++inode_count;

	inode->id = nid;
	inode->type = type;
	inode->fs = fs;

	if (inode->type == INODE_DIRECTORY) // Create child table if it's a directory
		strtable_create(&inode->children);

	table_add(&inode_table, nid, inode); // Add it to the inode table

	return 0;
}

void vfs_inode_delete(inode_t* inode)
{
	dealloc(inode);
}

void vfs_display()
{
	for (size_t i = 0; i < fs_table.size; i ++)
	{
		fs_t* fs = table_nth(&fs_table, i);
		logf("Filesystem %u is '%s'\n", fs->id, fs->name);
	}

	for (size_t i = 0; i < inode_table.size; i ++)
	{
		inode_t* inode = table_nth(&inode_table, i);

		//logf("Inode %u with type %u belongs to filesystem at %p\n", inode->id, inode->type, inode->fs);

		if (inode->fs->inode_display != nullptr)
			inode->fs->inode_display(inode);
	}
}
