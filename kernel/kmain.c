//
// file : kmain.c
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

#include <tupai/dev/console.h>
#include <tupai/util/log.h>
#include <tupai/util/mem.h>
#include <tupai/util/str.h>
#include <tupai/mem/kheap.h>
#include <tupai/mem/phys.h>
#include <tupai/cpu.h>

void usermode_test();

void kearly()
{
	// Kernel heap
	kheap_init();
	phys_init();
}

void kmain()
{
	console_init();

	log("Hello, World! Welcome to the kernel!\n");

	cpu_enable_int();

	cpu_user_jump(&usermode_test);

	while (true)
		cpu_halt();
}

void usermode_test()
{
	log("Hello, World! Welcome to user mode!\n");
	while (1);
}
