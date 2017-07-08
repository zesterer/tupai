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
#include <tupai/sys/mmap.hpp>
#include <tupai/sys/thread.hpp>
#include <tupai/sys/initrd.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/sys/proc.hpp>

// Filesystem
#include <tupai/fs/vfs.hpp>
#include <tupai/vfs/vfs.hpp>

// Virtual devices
#include <tupai/dev/serial.hpp>
#include <tupai/dev/ps2.hpp>
#include <tupai/dev/kbd.hpp>
#include <tupai/dev/tty.hpp>

// Initial software
#include <tupai/shell.hpp>

// I/O
#include <tupai/util/out.hpp>
#include <tupai/util/in.hpp>

namespace tupai
{
	void early()
	{
		// Core system environment

		sys::kmem_init(); // Initiate kernel memory allocation
		sys::mmap_init(); // Initiate page map & frame allocation

		// Map kernel memory
		//sys::mmap_reserve(0, arch_get_kernel_end() - arch_get_offset(), sys::KERNEL_PROC_ID);
	}

	void main()
	{
		// At this point, we should have a stable environment with memory
		// protection, a heap, a page frame allocator, etc. all configured.
		// The methods through which this is done are platform-dependent.
		// Now, however, it's relatively safe to run most code.

		sys::threading_init(); // Initiate multi-threading
		sys::call_bind();      // Initiate SYSCALL routine

		vfs::vfs_init(); // Init virtual filesystem

		//fs::vfs_init();     // Initiate filesystem
		sys::initrd_init(); // Initiate initrd filesystems

		sys::proc_init();      // Initiate processes

		// Initiate virtual devices
		dev::serial_init();
		dev::tty_init();

		// Run the kernel shell
		sys::thread_create(shell_main, 0, nullptr, "shell");

		interrupt_enable(true); // Enable interrupts

		sys::thread_kill(sys::thread_get_id());
	}
}
