//
// file : multiboot.hpp
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

#ifndef TUPAI_X86_MULTIBOOT_HPP
#define TUPAI_X86_MULTIBOOT_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		struct mb_start_tag_t
		{
			uint32_t total_size;
			uint32_t reserved;
		} __attribute__((packed));

		struct mb_tag_header_t
		{
			uint32_t type;
			uint32_t size;
		} __attribute__((packed));

		struct mb_meminfo_t
		{
			mb_tag_header_t head;
			uint32_t lower;
			uint32_t upper;
		} __attribute__((packed));

		struct mb_biosdev_t
		{
			mb_tag_header_t head;
			uint32_t biosdev;
			uint32_t part;
			uint32_t sub_part;
		} __attribute__((packed));

		struct mb_bootcmd_t
		{
			mb_tag_header_t head;
			uint8_t  cmd_start;
		} __attribute__((packed));

		struct mb_module_t
		{
			mb_tag_header_t head;
			uint32_t start;
			uint32_t end;
			uint8_t  args_start;
		} __attribute__((packed));

		void multiboot_set_header(uint64_t magic, void* header);

		mb_meminfo_t multiboot_get_meminfo();
	}
}

#endif
