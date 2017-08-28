//
// file : ramdisk.cpp
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
#include <tupai/sys/ramdisk.hpp>
#include <tupai/mem/mmap.hpp>
#include <tupai/task/task.hpp>
#include <tupai/sys/tmpfs.hpp>
#include <tupai/util/tar.hpp>
#include <tupai/arch.hpp>

#include <tupai/util/log.hpp>

namespace tupai
{
	namespace sys
	{
		namespace ramdisk
		{
			struct ramdisk_t
			{
				void*       start = nullptr;
				size_t      size = 0;
				const char* args = nullptr;
			};

			static const size_t RAMDISK_MAX = 32;
			static ramdisk_t ramdisks[RAMDISK_MAX];

			void add(void* start, size_t size, const char* args)
			{
				// Search for empty cache
				for (size_t i = 0; i < RAMDISK_MAX; i ++)
				{
					if (ramdisks[i].size == 0)
					{
						ramdisks[i] = ramdisk_t();

						ramdisks[i].start = start;
						ramdisks[i].size  = size;
						ramdisks[i].args  = args;
						break;
					}
				}
			}

			void reserve()
			{
				for (size_t i = 0; i < RAMDISK_MAX; i ++)
				{
					if (ramdisks[i].size != 0)
						mem::mmap::reserve_region((void*)((size_t)ramdisks[i].start - arch_get_offset()), ramdisks[i].size, task::get_kernel(), (uint8_t)mem::mmap::page_flags::STATIC); // Reserve the ramdisk region
				}
			}

			void load()
			{
				for (size_t i = 0; i < RAMDISK_MAX; i ++)
				{
					if (ramdisks[i].size != 0)
					{
						util::tar_header_t* cheader = (util::tar_header_t*)ramdisks[i].start;
						while (cheader != nullptr)
						{
							const char* path = cheader->filename;

							vfs::inode_type type = vfs::inode_type::DIRECTORY;
							switch (cheader->typeflag)
							{
							case '0':
								type = vfs::inode_type::NORMAL_FILE;
								break;
							case '1':
								type = vfs::inode_type::HARD_LINK;
								break;
							case '2':
								type = vfs::inode_type::SYM_LINK;
								break;
							case '3':
								type = vfs::inode_type::CHAR_DEVICE;
								break;
							case '4':
								type = vfs::inode_type::BLOCK_DEVICE;
								break;
							case '6':
								type = vfs::inode_type::PIPE;
								break;
							case '5':
							default:
								break;
							}

							if (type != vfs::inode_type::DIRECTORY)
								tmpfs::create(path, type, (const uint8_t*)tar_data(cheader), tar_size(cheader));

							cheader = tar_next(cheader);
						}
					}
				}
			}
		}
	}
}
