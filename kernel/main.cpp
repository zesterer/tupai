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
#include <tupai/arch.hpp>
#include <tupai/interrupt.hpp>

// Core system environment
#include <tupai/sys/kmem.hpp>
#include <tupai/sys/pipe.hpp>
#include <tupai/sys/initrd.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/vfs/vfs.hpp>
#include <tupai/proc/proc.hpp>
#include <tupai/proc/scheduler.hpp>

// Virtual devices
#include <tupai/dev/serial.hpp>
#include <tupai/dev/ps2.hpp>
#include <tupai/dev/kbd.hpp>
#include <tupai/dev/tty.hpp>

// Initial software
#include <tupai/shell.hpp>

namespace tupai
{
	void early()
	{
		// Core system environment

		sys::kmem_init(); // Initiate kernel memory allocation
		//sys::mmap_init(); // Initiate page map & frame allocation

		// Map kernel memory
		//sys::mmap_reserve(0, arch_get_kernel_end() - arch_get_offset(), sys::KERNEL_PROC_ID);
	}

	void main()
	{
		// At this point, we should have a stable environment with memory
		// protection, a heap, a page frame allocator, etc. all configured.
		// The methods through which this is done are platform-dependent.
		// Now, however, it's relatively safe to run most code.

		proc::scheduler_init();
		vfs::init();  // Init virtual filesystem

		proc::init(); // Initiate processes
		sys::call_bind();      // Initiate SYSCALL routine

		sys::initrd_init(); // Initiate initrd filesystems

		// Initiate virtual devices
		dev::serial_init();
		dev::tty_init();

		// Spawn a shell process
		proc::create("shell", vfs::get_root()).spawn_thread(shell_main);

		interrupt_enable(true); // Enable interrupts

		while (true);
	}
}
