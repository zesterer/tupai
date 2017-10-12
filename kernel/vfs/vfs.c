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

static void inode_delete(inode_t* inode);

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

	// TODO : Uncomment this
	//fs->root = ALLOC_OBJ(inode_t); // Create a root directory inode
	//vfs_inode_create(fs->root, fs, INODE_DIRECTORY);

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
	inode_delete(fs->root);

	dealloc(fs);
}

int vfs_inode_create(inode_t* inode, fs_t* fs, int type, inode_t* base, const char* path)
{
	id_t nid = ++inode_count;

	inode->id = nid;
	inode->fs = fs;
	inode->type = type;
	inode->ref = 0;
	inode->internal = nullptr;

	if (fs->inode_create_event(fs, inode) == FS_PROPAGATE)
	{
		if (inode->type == INODE_DIRECTORY)
			strtable_create(&inode->children);

		table_add(&inode_table, nid, inode);
		vfs_inode_mount(inode, base, path);
		return 0;
	}
	else
	{
		inode_delete(inode);
		return 1;
	}
}

int vfs_inode_remove(inode_t* base, const char* path);

int vfs_inode_at(inode_t* base, const char* path, inode_t** ret);

int vfs_inode_mount(inode_t* inode, inode_t* base, const char* path)
{
	// TODO : Mount here
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

		if (inode->fs->inode_display_event != nullptr)
			if (inode->fs->inode_display_event(inode) == FS_PROPAGATE)
				logf("Inode %u with type %u belongs to filesystem at %p\n", inode->id, inode->type, inode->fs);
	}
}

static void inode_delete(inode_t* inode)
{
	strtable_delete(&inode->children);
	dealloc(inode);
}

/*
int vfs_inode_create(inode_t* inode, inode_t* base, int type, const char* path)
{
	id_t nid = ++inode_count;

	inode->id = nid;
	inode->type = type;
	inode->fs = fs;
	inode->ref = 1;

	if (inode->type == INODE_DIRECTORY) // Create child table if it's a directory
		strtable_create(&inode->children);

	if (vfs_inode_mount(inode, base, path);

	table_add(&inode_table, nid, inode); // Add it to the inode table

	return 0;
}

int vfs_inode_remove(inode_t* base, const char* path)
{
	inode_t* parent = base;
	char child_name[ELEMENT_MAX_LEN + 1];

	char parent_path[PATH_MAX_LEN + 1];
	if (path_extract_base(path, parent_path)) // Base is already parent
	{
		if (path_extract(path, path_elements(path) - 1, child_name)) // No child in path
			return 2;
	}
	else
	{
		if (path_extract(path, path_elements(path) - 1, child_name)) // Something is seriously wrong
			return 2;
		else
		{
			if (vfs_inode_at(base, parent_path, &parent))
				return 1;
		}
	}

	inode_t* child = strtable_get(&base->children, child_name);
	if (child == nullptr)
		return 1;
	else
	{
		if (child->ref <= 1)
			vfs_inode_delete(child);
		else
			child->ref --;
	}
}

int vfs_inode_at(inode_t* base, const char* path, inode_t** ret);
int vfs_inode_mount(inode_t* inode, inode_t* base, const char* path);

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
*/
