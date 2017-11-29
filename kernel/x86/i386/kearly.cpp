//
// file : kearly.cpp
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

#include <tupai/x86/i386/gdt.hpp>
#include <tupai/x86/i386/idt.hpp>
#include <tupai/x86/i386/isr.hpp>
#include <tupai/x86/pic.hpp>
#include <tupai/x86/kbd.hpp>
#include <tupai/x86/vga/textmode.hpp>
#include <tupai/util/type.hpp>

namespace tupai::x86::i386
{
	extern "C" void kearly(uintptr_t mb_header)
	{
		(void)mb_header;

		// Init basic textmode terminal
		vga::textmode::init();

		// Init CPU tables
		gdt::init();
		idt::init();

		// Initiate ISRs
		isr::init();

		// Init interrupt hardware
		pic::init();

		// Init non-critical hardware
		kbd::init();
	}
}
