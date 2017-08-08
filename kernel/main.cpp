//
// file : main.cpp
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

// Tupai
#include <tupai/main.hpp>

// Low-level utilities
#include <tupai/arch.hpp>
#include <tupai/cpu.hpp>
#include <tupai/irq.hpp>

// Core setup
#include <tupai/mem/mmap.hpp>
#include <tupai/mem/kmem.hpp>

// Core systems
#include <tupai/vfs/vfs.hpp>
#include <tupai/proc/proc.hpp>
#include <tupai/proc/scheduler.hpp>
#include <tupai/sys/call.hpp>

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

		// Core setup
		// ----------
		// These are things that are required to properly execute code
		mem::mmap::init(); // Initiate memory maps
		mem::mmap::reserve_region( // Reserve kernel memory
			(void*)arch_get_kernel_start(),
			arch_get_kernel_end() - arch_get_kernel_start(),
			proc::get_kernel(),
			(uint8_t)mem::mmap::page_flags::NONE
		);
		mem::kmem::init(); // Initiate kernel memory allocation
	}

	void main()
	{
		// At this point, we should have a stable environment with memory
		// protection, a heap, a page frame allocator, etc. all configured.
		// The methods through which this is done are platform-dependent.
		// Now, however, it's relatively safe to run most code.

		// Core systems
		// ------------
		// These are things required to run kernel threads, processes, manipulate files, etc.
		vfs::init();            // Initiate virtual filesystem
		proc::init();           // Initiate processes
		proc::scheduler_init(); // Initiate scheduler
		sys::call_init();       // Initiate syscalls

		// Essential systems
		// -----------------
		// These are things that are essential to the running of userland systems
		sys::ramdisk_init(); // Initiate initrd filesystems

		// Interfaces
		// ----------
		// These are non-essential interfaces to system devices
		dev::serial_init(); // Initiate the kernel's serial driver
		dev::tty_init();    // Initiate the kernel's tty

		// We're finally ready to start the init process.
		proc::create("init", vfs::get_root()).spawn_thread(init);

		// Enable interrupts and wait for the scheduler to kick in
		irq::enable();
		cpu::halt();
	}

	void init(int argc, char* argv[])
	{
		(void)argc;
		(void)argv;

		// Spawn a kernel shell process
		proc::create("shell", vfs::get_root()).spawn_thread(shell_main);
	}
}
