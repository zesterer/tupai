/*
* 	file : vga.cpp
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

// Tupai
#include <tupai/x86_family/vga.hpp>
#include <tupai/x86_family/multiboot.hpp>
#include <tupai/i686/i686.hpp>

#include <tupai/util/conv.hpp>
#include <tupai/kdebug.hpp>

namespace tupai
{
	namespace x86_family
	{
		extern "C" byte _binary_screenfont_psfu_start;

		static inline uint32 color_blend(uint32 lower, uint32 higher);
		static void blit_character(uint32 c, uint16 x, uint16 y, uint32 fg_color, uint32 bg_color);

		struct psf2_header
		{
			// Magic heaader data
			static const byte MAGIC0 = 0x72;
			static const byte MAGIC1 = 0xb5;
			static const byte MAGIC2 = 0x4a;
			static const byte MAGIC3 = 0x86;

			// Bitflags
			static const byte HAS_UNICODE_TABLE = 0x01;

			// Supported versions
			static const byte MAX_VERSION = 0x00;

			// UTF-8 separators
			static const byte SEPARATOR = 0xFF;
			static const byte STARTSEQ  = 0xFE;

			uint8  magic[4];
			uint32 version;
			uint32 header_size;
			uint32 flags;
			uint32 glyph_num;
			uint32 glyph_size;
			uint32 height;
			uint32 width;
		};

		struct vga_config
		{
			enum class framebuffer_type
			{
				INDEXED = 0,
				RGB = 1,
				EGA_TEXT = 2,
			};

			uint32*           fb_addr;
			uint32            fb_pitch;
			uint32            fb_width;
			uint32            fb_height;
			uint8             fb_bpp;
			framebuffer_type  fb_type;
		};

		bool vga_initiated = false;
		vga_config config;

		virtualtty vga_virtualtty;
		uint32 vga_virtualtty_change_counter = 0;
		static void vga_virtualtty_changed();

		bool vga_init()
		{
			multiboot_header::framebuffer fb = multiboot_get_framebuffer();

			config.fb_addr = (uint32*)fb.address;
			config.fb_pitch = fb.pitch;
			config.fb_width = fb.width;
			config.fb_height = fb.height;
			config.fb_bpp = fb.bpp;
			config.fb_type = (vga_config::framebuffer_type)fb.type;

			vga_virtualtty = virtualtty_create(config.fb_width / 8, config.fb_height / 16);
			vga_virtualtty.change_signal_func = vga_virtualtty_changed;

			vga_initiated = true;
			klog_init("Initiated VGA", vga_initiated);

			vga_textmode_clear();

			return vga_initiated;
		}

		safeval<console_config> vga_get_console_config()
		{
			console_config cfg;

			cfg.columns = config.fb_width / 8;
			cfg.rows = config.fb_height / 16;

			if (vga_initiated)
				return safeval<console_config>(cfg, true);
			else
				return safeval<console_config>(cfg, false);
		}

		safeptr<virtualtty> vga_get_virtualtty()
		{
			if (!vga_initiated)
				return safeptr<virtualtty>(nullptr, false);

			return safeptr<virtualtty>(&vga_virtualtty, true);
		}

		static void vga_virtualtty_changed()
		{
			if (!vga_initiated)
				return;

			for (uint16 i = 0; i < vga_virtualtty.cols; i ++)
			{
				for (uint16 j = 0; j < vga_virtualtty.rows; j ++)
				{
					uint32 pos = j * vga_virtualtty.cols + i;
					ttyentry entry = vga_virtualtty.buffer[pos];

					if (entry.change_stamp < vga_virtualtty_change_counter)
						continue;

					blit_character(entry.c, i, j, 0xFFFFFFFF, 0x00000000);
				}
			}

			vga_virtualtty_change_counter = vga_virtualtty.change_counter;
		}

		static inline uint32 color_blend(uint32 lower, uint32 upper)
		{
			byte a_upper = (upper >> 24) & 0xFF;

			byte r_lower = (lower >> 16) & 0xFF;
			byte g_lower = (lower >> 8)  & 0xFF;
			byte b_lower = (lower >> 0)  & 0xFF;

			byte r_upper = (upper >> 16) & 0xFF;
			byte g_upper = (upper >> 8)  & 0xFF;
			byte b_upper = (upper >> 0)  & 0xFF;

			byte r_final = (r_upper * a_upper + r_lower * (255 - a_upper)) >> 8;
			byte g_final = (g_upper * a_upper + g_lower * (255 - a_upper)) >> 8;
			byte b_final = (b_upper * a_upper + b_lower * (255 - a_upper)) >> 8;

			return (0xFF << 24) | (r_final << 16) | (g_final << 8) | b_final << 0;
		}

		static void blit_character(uint32 c, uint16 x, uint16 y, uint32 fg_color, uint32 bg_color)
		{
			if (config.fb_type != vga_config::framebuffer_type::RGB)
				return;

			psf2_header* header = (psf2_header*)&_binary_screenfont_psfu_start;

			uint8* glyph = (uint8*)((umem)&_binary_screenfont_psfu_start + header->header_size + ((c > 0 && c < header->glyph_num) ? c : 0) * header->glyph_size);

			bg_color = (bg_color & 0x00FFFFFF) | 0x40000000;

			int offx = x * header->width;
			int offy = y * header->height;
			int w = config.fb_width;
			int h = config.fb_height;
			for (int j = 0; j < header->height; j ++)
			{
				for (int i = 0; i < header->width; i ++)
				{
					if ((*glyph >> (header->width - i)) & 0x01 > 0)
						config.fb_addr[(config.fb_pitch >> 2) * (offy + j) + (offx + i)] = fg_color;
					else
						config.fb_addr[(config.fb_pitch >> 2) * (offy + j) + (offx + i)] = color_blend(((((offx + i) * 256) / w) & 0xFF) + (((((offy + j) * 256) / h) & 0xFF) << 16), bg_color);
				}

				glyph += (header->width + 7) / 8;
			}
		}

		void vga_textmode_put_char(char c, uint16 col, uint16 row)
		{
			if (!vga_initiated)
				return;

			blit_character(c, col, row, 0xFFFFFFFF, 0x00000000);
		}

		void vga_textmode_clear()
		{
			for (int i = 0; i < 80; i ++)
			{
				for (int j = 0; j < 25; j ++)
				{
					vga_textmode_put_char(' ', i, j);
				}
			}
		}
	}
}
