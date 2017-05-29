//
// file : multiboot.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/x86/multiboot.hpp>
#include <tupai/util/math.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/str.hpp>
#include <tupai/sys/initrd.hpp>
#include <tupai/arch.hpp>
#include <tupai/debug.hpp>

namespace tupai
{
	namespace x86
	{
		static const uint64_t MULTIBOOT_EAX_MAGIC = 0x36D76289;
		static const int      MAX_TAGS            = 1024;

		static void multiboot_parse_meminfo(mb_tag_header_t* tag);
		static void multiboot_parse_biosdev(mb_tag_header_t* tag);
		static void multiboot_parse_bootcmd(mb_tag_header_t* tag);
		static void multiboot_parse_module (mb_tag_header_t* tag);

		void multiboot_set_header(uint64_t magic, void* header)
		{
			if (magic != MULTIBOOT_EAX_MAGIC)
			{
				debug_println("Warning: Multiboot magic constant is not correct!");
				return;
			}

			mb_start_tag_t* start = (mb_start_tag_t*)((size_t)header + arch_get_offset());
			uint32_t total_size = (uint32_t)start->total_size;
			debug_println("Multiboot total header size is ", total_size);

			mb_tag_header_t* ctag = (mb_tag_header_t*)util::align_ceiling((size_t)start + sizeof(mb_start_tag_t), 8);

			debug_println("Parsing Multiboot tags...");

			int i = 0;
			while ((size_t)ctag - (size_t)start < total_size && i < MAX_TAGS)
			{
				//debug_println("Multiboot tag at ", ctag, " of size ", (uint32_t)ctag->size, " and type ", (uint32_t)ctag->type);

				switch(ctag->type)
				{
				case 4:
					multiboot_parse_meminfo(ctag);
					break;
				case 5:
					multiboot_parse_biosdev(ctag);
					break;
				case 1:
					multiboot_parse_bootcmd(ctag);
					break;
				case 3:
					multiboot_parse_module(ctag);
					break;
				default:
					break;
				}

				// Increment tag pointer
				ctag = (mb_tag_header_t*)util::align_ceiling((size_t)ctag + ctag->size, 8);
				i ++;
			}
		}

		void multiboot_parse_meminfo(mb_tag_header_t* tag)
		{
			struct mb_meminfo_t
			{
				mb_tag_header_t head;
				uint32_t lower;
				uint32_t upper;
			} __attribute__((packed));

			mb_meminfo_t* meminfo = (mb_meminfo_t*)tag;

			debug_println("Found MemInfo tag! lower = ", (void*)meminfo->lower, ", upper = ", (void*)meminfo->upper);
		}

		void multiboot_parse_biosdev(mb_tag_header_t* tag)
		{
			struct mb_biosdev_t
			{
				mb_tag_header_t head;
				uint32_t biosdev;
				uint32_t part;
				uint32_t sub_part;
			} __attribute__((packed));

			mb_biosdev_t* biosdev = (mb_biosdev_t*)tag;

			debug_println("Found BIOSDev tag! biosdev = ", (uint32_t)biosdev->biosdev, ", part = ", (uint32_t)biosdev->part, ", sub_part = ", (uint32_t)biosdev->sub_part);
		}

		void multiboot_parse_bootcmd(mb_tag_header_t* tag)
		{
			struct mb_bootcmd_t
			{
				mb_tag_header_t head;
				uint8_t  cmd_start;
			} __attribute__((packed));

			mb_bootcmd_t* bootcmd = (mb_bootcmd_t*)tag;

			debug_println("Found BootCMD tag! cmd = ", (const char*)&bootcmd->cmd_start);
		}

		void multiboot_parse_module(mb_tag_header_t* tag)
		{
			struct mb_module_t
			{
				mb_tag_header_t head;
				uint32_t start;
				uint32_t end;
				uint8_t  args_start;
			} __attribute__((packed));

			mb_module_t* module = (mb_module_t*)tag;

			debug_println("Found Module tag! start = ", (void*)module->start, ", part = ", (void*)module->end, ", args = ", (const char*)&module->args_start);

			// If the module is an initrd, add it to the cache
			if (util::str_equal((const char*)&module->args_start, "initrd"))
				sys::initrd_cache_add((void*)module->start + arch_get_offset(), (size_t)(module->end - module->start + 1) + arch_get_offset(), (const char*)&module->args_start);
		}
	}
}
