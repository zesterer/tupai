//
// file : mb.c
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

#include <tupai/arch/x86/mb.h>
#include <tupai/util/log.h>
#include <tupai/util/mem.h>
#include <tupai/util/args.h>
#include <tupai/util/str.h>
#include <tupai/mem/phys.h>
#include <tupai/mem/virt.h>
#include <tupai/rd.h>
#include <tupai/def.h>

typedef struct tag_head
{
	uint32_t type;
	uint32_t size;
} __attribute__((packed)) tag_head_t;

typedef struct tag_args
{
	tag_head_t head;
	char args_start;
} __attribute__((packed)) tag_args_t;

typedef struct tag_bln
{
	tag_head_t head;
	char name_start;
} __attribute__((packed)) tag_bln_t;

typedef struct tag_module
{
	tag_head_t head;
	uint32_t start;
	uint32_t end;
	char args_start;
} __attribute__((packed)) tag_module_t;

typedef struct tag_meminfo
{
	tag_head_t head;
	uint32_t lower;
	uint32_t upper;
} __attribute__((packed)) tag_meminfo_t;

typedef struct mmap_entry
{
	uint64_t offset;
	uint64_t size;
	uint32_t type;
	uint32_t reserved;
} __attribute__((packed)) mmap_entry_t;

typedef struct tag_mmap
{
	tag_head_t head;
	uint32_t entry_size;
	uint32_t entry_version;
	mmap_entry_t entries_start;
} __attribute__((packed)) tag_mmap_t;

typedef struct tag_efi32
{
	tag_head_t head;
	uint32_t ptr;
} __attribute__((packed)) tag_efi32_t;

typedef struct tag_efi64
{
	tag_head_t head;
	uint64_t ptr;
} __attribute__((packed)) tag_efi64_t;

static void parse_args(tag_args_t* bln); // Kernel arguments
static void parse_bln(tag_bln_t* bln); // Bootloader Name
static void parse_module(tag_module_t* bln); // Module
static void parse_meminfo(tag_meminfo_t* meminfo); // Basic memory information
static void parse_mmap(tag_mmap_t* mmap); // Memory map
static void parse_efi32(tag_efi32_t* efi32); // 32-bit EFI system table
static void parse_efi64(tag_efi64_t* efi64); // 64-bit EFI system table

void mb_parse(uintptr_t header)
{
	log("[ OK ] Parsing Multiboot data...\n");

	tag_head_t* p = (tag_head_t*)(header + 8); // Skip start tag
	while (p->type != 0) // Keep going until end tag is reached
	{
		switch (p->type) // Which tag type are we dealing with?
		{
			case 1:
				parse_args((tag_args_t*)p);
				break;
			case 2:
				parse_bln((tag_bln_t*)p);
				break;
			case 3:
				parse_module((tag_module_t*)p);
				break;
			case 4:
				parse_meminfo((tag_meminfo_t*)p);
				break;
			case 6:
				parse_mmap((tag_mmap_t*)p);
				break;
			case 11:
				parse_efi32((tag_efi32_t*)p);
				break;
			case 12:
				parse_efi64((tag_efi64_t*)p);
				break;
			default:
				logf("[ OK ] Found unrecognised Multiboot entry with type %u\n", (uint)p->type);
				break;
		}

		// Increment to next tag
		p += align_up(p->size, 8) / sizeof(*p);
	}

	log("[ OK ] Finished parsing Multiboot data\n");
}

void mb_reserve()
{
	// Map Multiboot data to the higher half
	phys_set_region(0, (size_t)kernel_end, KERNEL | MOVABLE | RAM | USED, nullptr); // Reserve kernel memory
	virt_map_region(&kvirt, VIRT_OFFSET, 0, (size_t)kernel_end, 0); // Map higher-half addresses to lower kernel memory
}

void parse_args(tag_args_t* tag)
{
	logf("[ OK ] Kernel arguments are '%s'\n", &tag->args_start);
}

void parse_bln(tag_bln_t* tag)
{
	logf("[ OK ] Bootloader name is '%s'\n", &tag->name_start);
}

void parse_module(tag_module_t* tag)
{
	char typebuff[64];
	if (find_arg(&tag->args_start, "type", typebuff, sizeof(typebuff) / sizeof(char)) == 0)
	{
		logf("[ OK ] Found '%s' module with args '%s' at %p\n", typebuff, &tag->args_start, (void*)(uintptr_t)tag->start);

		if (str_equal(typebuff, "initrd"))
		{
			rd_preload((uintptr_t)tag->start, (size_t)(tag->end - tag->start), RD_TAR, &tag->args_start);
			logf("[ OK ] Module at %p is an initrd. Load cached until later.\n", (void*)(uintptr_t)tag->start);
		}
	}
	else
		logf("[ OK ] Found unrecognised module with args '%s' at %p\n", &tag->args_start, (void*)(uintptr_t)tag->start);
}

void parse_meminfo(tag_meminfo_t* meminfo)
{
	logf("[ OK ] Reported lower memory is %u KiB\n", (uint)meminfo->lower);
	logf("[ OK ] Reported upper memory is %u KiB\n", (uint)meminfo->upper);

	phys_preload_size = meminfo->upper * 1024 + MEM_UPPER_START;
	log("[ OK ] Physical memory preload set from reported meminfo data\n");
}

void parse_mmap(tag_mmap_t* mmap)
{
	if (mmap->entry_version != 0)
		log("[ OK ] Found non-compliant memory map\n");
	else
	{
		size_t entry_count = (mmap->head.size - sizeof(tag_mmap_t)) / sizeof(mmap_entry_t) + 1;
		logf("[ OK ] Reading memory map with %u entries...\n", (uint)entry_count);

		for (size_t i = 0; i < entry_count; i ++)
		{
			mmap_entry_t* centry = &(&mmap->entries_start)[i];

			const char* type_str = "UNKNOWN";
			switch (centry->type)
			{
				case 1:
					type_str = "RAM";
					break;
				case 2:
					type_str = "RESERVED";
					break;
				case 3:
					type_str = "ACPI";
					break;
				case 4:
					type_str = "PRESERVE";
					break;
				case 5:
					type_str = "DEFECTIVE";
					break;
				default:
					break;
			}

			logf("[ OK ] Found region (type = %s, offset = %p, size = %X)\n", type_str, (void*)(uintptr_t)centry->offset, (uint)centry->size);
		}
	}
}

void parse_efi32(tag_efi32_t* efi32)
{
	logf("[ OK ] Found 32-bit EFI system table at %p\n", (void*)(uintptr_t)efi32->ptr);
}

void parse_efi64(tag_efi64_t* efi64)
{
	logf("[ OK ] Found 64-bit EFI system table at %p\n", (void*)(uintptr_t)efi64->ptr);
}