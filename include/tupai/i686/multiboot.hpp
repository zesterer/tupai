/*
* 	file : multiboot.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_I686_MULTIBOOT_HPP
#define TUPAI_I686_MULTIBOOT_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	struct MultibootHeader
	{
		// Multiboot info version number
		uint32 flags;

		// Available memory from BIOS
		uint32 mem_lower;
		uint32 mem_upper;

		// 'root' partition
		uint32 boot_device;

		// Kernel command-line
		uint32 cmd_line;

		// Boot module list
		uint32 module_count;
		uint32 module_address;

		// Elf section header
		uint32 elf_num;
		uint32 elf_size;
		uint32 elf_address;
		uint32 elf_shndx;

		// Memory mapping buffer
		uint32 mmap_length;
		uint32 mmap_address;

		// Drive info buffer
		uint32 drives_length;
		uint32 drives_address;

		// ROM configuration table
		uint32 config_table;

		// Bootloader name
		uint32 bootloader_name;

		// APM table
		uint32 apm_table;

		// Video
		uint32 vbe_control_info;
		uint32 vbe_mode_info;
		uint16 vbe_mode;
		uint16 vbe_interface_seg;
		uint16 vbe_interface_off;
		uint16 vbe_interface_len;

		// Framebuffer
		uint64 framebuffer_address;
		uint32 framebuffer_pitch;
		uint32 framebuffer_width;
		uint32 framebuffer_height;
		uint8 framebuffer_bpp;
		uint8 framebuffer_type;

		// Framebuffer palette
		uint32 framebuffer_palette_address;
		uint16 framebuffer_palette_num_colors;
	};
}

#endif
