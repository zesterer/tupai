//
// file : vfs.h
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

#ifndef TUPAI_VFS_VFS_H
#define TUPAI_VFS_VFS_H

#include <tupai/vfs/inode.h>
#include <tupai/vfs/fs.h>

void vfs_init();
int vfs_fs_create(fs_t* fs, const char* name);
void vfs_fs_delete(fs_t* fs);

int vfs_inode_create(inode_t* inode, fs_t* fs, int type, inode_t* base, const char* path);
int vfs_inode_remove(inode_t* base, const char* path);

int vfs_inode_at(inode_t* base, const char* path, inode_t** ret);
int vfs_inode_mount(inode_t* inode, inode_t* base, const char* path);

void vfs_display();

#endif
