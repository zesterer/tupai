//
// file : multiboot.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
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
