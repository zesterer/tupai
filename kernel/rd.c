//
// file : rd.c
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

#include <tupai/rd.h>
#include <tupai/util/log.h>
#include <tupai/util/tar.h>
#include <tupai/util/mem.h>
#include <tupai/mem/phys.h>
#include <tupai/mem/virt.h>
#include <tupai/mem/kmem.h>
#include <tupai/vfs/vfs.h>
#include <tupai/fs/tmpfs.h>
#include <tupai/def.h>

#define CACHE_SIZE 16

static rd_t rd_cache[CACHE_SIZE];
static size_t rd_count = 0;

static int load_tar(rd_t* rd);

void rd_preload(uintptr_t offset, size_t size, int type, const char* args)
{
	if ((uintptr_t)args < VIRT_OFFSET)
		args = (const char*)(VIRT_OFFSET + (uintptr_t)args);

	rd_cache[rd_count++] = (rd_t){ .offset = offset, .size = size, .type = type, args = args };
}

void rd_preserve()
{
	for (size_t i = 0; i < rd_count; i ++)
	{
		phys_set_region(rd_cache[i].offset, rd_cache[i].size, KERNEL | MOVABLE | RAM | USED, nullptr); // Reserve kernel memory

		uintptr_t noffset = kmem_new_region(rd_cache[i].size);
		virt_map_region(&kvirt, noffset, rd_cache[i].offset, rd_cache[i].size, 0); // Map higher-half addresses to lower kernel memory
		rd_cache[i].offset = noffset + rd_cache[i].offset % PAGE_SIZE;
	}

	virt_switch(&kvirt); // Update virtual address space

	logf("[ OK ] Preserved %u ramdisks to higher virtual memory\n", rd_count);
}

void rd_init()
{
	log("[ OK ] Picking up ramdisk cache...\n");

	for (size_t i = 0; i < rd_count; i ++)
	{
		switch (rd_cache[i].type)
		{
			case RD_TAR:
				load_tar(&rd_cache[i]);
				break;

			default:
				logf("[ OK ] Loading unrecognised ramdisk at %p...\n", (void*)rd_cache[i].offset);
				break;
		}
	}
}

int load_tar(rd_t* rd)
{
	logf("[ OK ] Loading tar ramdisk at %p...\n", (void*)rd->offset);

	// Create a new tmpfs filesystem for the tar ramdisk
	fs_t* nfs = ALLOC_OBJ(fs_t);
	tmpfs_create(nfs);

	for (tar_entry_t* tar = (tar_entry_t*)rd->offset; tar != nullptr; tar = tar_next(tar))
	{
		//logf("[ OK ] Found tar file with name '%s'\n", tar->filename);

		int type;
		switch (tar->typeflag) // TODO : Expand this
		{
			case '0':
				type = INODE_NORMAL;
				break;
			case '5':
				type = INODE_DIRECTORY;
				break;
			default:
				type = INODE_NORMAL;
				break;
		}

		// TODO : Uncomment this
		inode_t* ninode = ALLOC_OBJ(inode_t);
		int val = vfs_inode_create(ninode, nfs, type, nfs->root, (const char*)tar->filename);

		if (val)
			logf("Failed to create %s\n", tar->filename);
		else
			logf("Created %s, type %u\n", tar->filename, type);
		//nfs->inode_set_raw(ninode, tar_data(tar), tar_size(tar));
	}

	return 0;
}
