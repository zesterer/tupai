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

static void parse_args(tag_args_t* bln); // Kernel arguments
static void parse_bln(tag_bln_t* bln); // Bootloader Name
static void parse_module(tag_module_t* bln); // Module

void mb_parse(size_t header)
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
			default:
				break;
		}

		// Increment to next tag
		p += align_up(p->size, 8) / sizeof(*p);
	}

	while (1);
}

void parse_args(tag_args_t* tag)
{
	log("[ OK ] Kernel arguments are '%s'\n", &tag->args_start);
}

void parse_bln(tag_bln_t* tag)
{
	log("[ OK ] Bootloader name is '%s'\n", &tag->name_start);
}

void parse_module(tag_module_t* tag)
{
	log("[ OK ] Found module with args '%s' starting at %p\n", &tag->args_start, (void*)tag->start);
}
