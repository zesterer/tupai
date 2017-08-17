//
// file : kentry.cpp
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
#include <tupai/main.hpp>
#include <tupai/debug.hpp>
#include <tupai/arch.hpp>
#include <tupai/x86/multiboot.hpp>
#include <tupai/x86/i386/gdt.hpp>
#include <tupai/x86/i386/idt.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		namespace i386
		{
			// Unions are used here SPECIFICALLY to avoid global construction!
			union MBMagic  { uint32_t value; MBMagic()  {} } mb_magic_tmp;
			union MBHeader { void* value;    MBHeader() {} } mb_header_tmp;

			extern "C" void kentry(uint32_t mb_magic, void* mb_header, void* stack)
			{
				// Save Multiboot data
				mb_magic_tmp.value = mb_magic;
				mb_header_tmp.value = mb_header;

				// Initiate debugging
				debug_init();

				// Passed information
				debug_print("Kernel virtual base is located at offset ", (void*)arch_get_offset(), '\n');
				debug_print( // Display kentry info
					"kentry at ", (void*)&kentry, " called with:\n",
					"  mb_magic  -> ", util::fmt_int<uint32_t>(mb_magic, 16), '\n',
					"  mb_header -> ", mb_header, '\n',
					"  stack     -> ", stack, '\n'
				);

				// Core system setup
				early();
			}

			extern "C" void kmain()
			{
				// Load multiboot information
				multiboot_set_header(mb_magic_tmp.value, mb_header_tmp.value);

				// Initiate and install the GDT
				gdt_init();
				gdt_install();

				// Initiate and install the IDT
				idt_init();
				idt_install();

				// x86 initiation
				arch_init();

				debug_print("Finished i386 initiation\n");

				main(); // Main kernel entry
			}
		}
	}
}
