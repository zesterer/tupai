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
#include <tupai/util/path.h>
#include <tupai/mem/kheap.h>
#include <tupai/mem/phys.h>
#include <tupai/mem/virt.h>
#include <tupai/mem/kmem.h>
#include <tupai/vfs/vfs.h>
#include <tupai/kshell.h>
#include <tupai/rd.h>
#include <tupai/cpu.h>
#include <tupai/def.h>

void usermode_test();

void kearly()
{
	// Initiate kernel offset memory
	kmem_init();

	// Kernel heap
	kheap_init();
	phys_init();

	virt_create(&kvirt);

	// Set up higher-half paging
	phys_set_region(0, (size_t)kernel_end, KERNEL | MOVABLE | RAM | USED, nullptr); // Reserve kernel memory
	virt_map_region(&kvirt, VIRT_OFFSET, 0, (size_t)kernel_end, 0); // Map higher-half addresses to lower kernel memory
	virt_switch(&kvirt); // Switch to the new virtual memory addresses

	// Preserve ramdisks
	rd_preserve();

	logf("[ OK ] Reserved kernel memory between %p and %p\n", 0, (void*)kernel_end);
}

void kmain()
{
	log("[ OK ] Entered the kernel main\n");

	// Core systems initiation //
	// ----------------------- //

	vfs_init();
	console_init();

	// Secondary setup //
	// --------------- //

	rd_init();

	// Initiation finished, show tests //
	// ------------------------------- //

	//phys_display();
	//vfs_display();

	// Prepare the CPU for the scheduler //
	// --------------------------------- //

	cpu_enable_int();

	kshell_main(0, nullptr);

	while (true)
		cpu_halt();
}

void usermode_test()
{
	log("Hello, World! Welcome to user mode!\n");
	while (1);
}
