//
// file : main.cpp
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
#include <tupai/panic.hpp>

// Low-level utilities
#include <tupai/arch.hpp>
#include <tupai/cpu.hpp>
#include <tupai/irq.hpp>

// Core setup
#include <tupai/mem/mmap.hpp>
#include <tupai/mem/kmem.hpp>

// Core systems
#include <tupai/vfs/vfs.hpp>
#include <tupai/sys/pipe.hpp>
#include <tupai/sys/tmpfs.hpp>
#include <tupai/task/task.hpp>
#include <tupai/task/scheduler.hpp>
#include <tupai/call/call.hpp>

// Essential systems
#include <tupai/sys/ramdisk.hpp>

// Interfaces
#include <tupai/dev/serial.hpp>
#include <tupai/dev/ps2.hpp>
#include <tupai/dev/kbd.hpp>
#include <tupai/dev/tty.hpp>

// Initial software
#include <tupai/shell.hpp>

namespace tupai
{
	void init(int argc, char* argv[]);

	void early()
	{
		// *** WARNING ***
		// Code executed here should not rely on constructed states

		// Extremely early construction of static objects that must RETAIN state
		dev::tty_construct();

		// Core setup
		// ----------
		// These are things that are required to properly execute code
		mem::mmap::init(); // Initiate memory maps
		mem::mmap::reserve_region( // Reserve kernel memory
			(void*)arch_get_kernel_start(),
			arch_get_kernel_end() - arch_get_kernel_start(),
			task::get_kernel(),
			(uint8_t)mem::mmap::page_flags::NONE
		);
		sys::ramdisk::reserve(); // Reserve ramdisk memory

		mem::kmem::init(); // Initiate kernel memory allocation
	}

	void main()
	{
		// At this point, we should have a stable environment with memory
		// protection, a heap, a page frame allocator, etc. all configured.
		// The methods through which this is done are platform-dependent.
		// Now, however, it's relatively safe to run most code.

		// Core hardware
		// -------------
		// These are platform-dependent hardware components needed for a stable kernel
		arch_init();

		// Core systems
		// ------------
		// These are things required to run kernel threads, processes, manipulate files, etc.
		vfs::init();            // Initiate virtual filesystem
		sys::tmpfs::init();     // Initiate temporary ram filesystem
		sys::pipe::init();      // Initiate pipes
		task::init();           // Initiate processes
		task::scheduler_init(); // Initiate scheduler
		call::init();       // Initiate syscalls

		// Essential systems
		// -----------------
		// These are things that are essential to the running of userland systems
		sys::ramdisk::load(); // Load initrd filesystems

		// Interfaces
		// ----------
		// These are non-essential interfaces to system devices
		dev::serial_init(); // Initiate the kernel's serial driver
		dev::tty_init();    // Initiate the kernel's tty

		// We're finally ready to start the init process.
		task::create_process("init", vfs::get_root()).spawn_thread(init);

		// Enable interrupts and wait for the scheduler to kick in
		irq::enable();
		cpu::halt();
	}

	void init(int argc, char* argv[])
	{
		(void)argc;
		(void)argv;

		// Spawn a kernel shell process
		task::proc_ptr_t shell = task::create_process("shell", vfs::get_root());

		// Give it appropriate stdio streams
		shell.create_fd(vfs::get_inode("/dev/stdin"),  0);
		shell.create_fd(vfs::get_inode("/dev/stdout"), 1);
		shell.create_fd(vfs::get_inode("/dev/stdout"), 2);

		// Spawn a kernel thread (note: execution may begin arbitrarily after this point!)
		shell.spawn_thread(shell_main);
	}
}
