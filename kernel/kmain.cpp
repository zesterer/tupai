/*
* 	file : kmain.cpp
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
#include <tupai/kmain.hpp>
#include <tupai/tty.hpp>
#include <tupai/prompt.hpp>
#include <tupai/mempool.hpp>
#include <tupai/syscall.hpp>
#include <tupai/task.hpp>
#include <tupai/kdebug.hpp>

#include <tupai/early/out.hpp>
#include <tupai/early/ansi.hpp>

#include <tupai/util/safetype.hpp>
#include <tupai/util/conv.hpp>
#include <tupai/util/str.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/kbd.hpp>
	#include <tupai/i686/pit.hpp>
	#include <tupai/i686/paging.hpp>
#endif

namespace tupai
{
	static void kernel_task_test_a();
	static void kernel_task_test_b();
	static void kernel_task_test_c();

	void kmain_write_check(const char* str, int error = 0)
	{
		tty_set_fg_color(tty_color::DEFAULT_FG);
		tty_write('[');

		if (error == 0)
		{
			tty_set_fg_color(0x0A);
			tty_write_str(" OK ");
		}
		else
		{
			tty_set_fg_color(0x0C);
			tty_write_str("FAIL");
		}

		tty_set_fg_color(tty_color::DEFAULT_FG);
		tty_write_str("] ");
		tty_write_str(str);
		tty_write('\n');
	}

	void kernel_welcome()
	{
		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			tty_clear();
		#else
			tty_write_str("\n");
		#endif

		tty_set_fg_color(tty_color::WHITE);
		tty_write_str("Welcome to ");
		tty_write_str(SYSTEM_NAME_DECORATIVE);
		tty_write_str(" ");
		tty_write_str(SYSTEM_VERSION_MAJOR);
		tty_write_str(".");
		tty_write_str(SYSTEM_VERSION_MINOR);
		tty_write_str(".");
		tty_write_str(SYSTEM_VERSION_RELEASE);
		tty_write_str("\n");

		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			const char GPL3_LICENCE_SHORT[] = "Copyright (C) 2016 Joshua Barretto\n\n" \
			"This program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are " \
			"welcome to\nredistribute it under certain conditions.\n\nYou should have received a copy" \
			" of the GNU\nGeneral Public License along with this program.\nIf not, see <http://www.gn" \
			"u.org/licenses/>.\n";

			tty_write('\n');
			tty_write_str(GPL3_LICENCE_SHORT);
		#endif

		tty_write('\n');
		tty_set_fg_color(tty_color::DEFAULT_FG);
	}

	// Kernel early
	void kearly()
	{
		early::ansi_init();

		//tty_init();
		kmain_write_check("Initiated VGA driver");
		kmain_write_check("Initiated kernel TTY");

		mempool_init((ubyte*)mempool_begin, mempool_size, 64); // At 2 MB, 1 MB in size, composed of blocks of 64 B
		kmain_write_check("Initiated dynamic memory pool");

		#if defined(SYSTEM_ARCH_i686)
			paging_init();
			kmain_write_check("Initiated Paging");
			paging_enable();
			kmain_write_check("Enabled Paging");

			gdt_init();
			kmain_write_check("Initiated GDT");

			idt_init();
			kmain_write_check("Initiated IDT");

			pit_init();
			kmain_write_check("Initiated PIT");

			kbd_init();
			kmain_write_check("Initiated keyboard");

			interrupt_enable();
			kmain_write_check("Enabled interrupts");
		#endif

		syscall_init();
		kmain_write_check("Initiated syscall routine");

		task_init();
		kmain_write_check("Initiated task scheduler");

		kmain_write_check("Kernel boot procedure complete");
	}

	// Kernel main
	void kmain()
	{
		///*
		// Get EFLAGS and CR3 TODO : Change this
		uint32 cr3 = 0;
		uint32 eflags = 0x202;

		//libk::printf("\nAdding test tasks A, B and C...\n");
		task_add("testa", kernel_task_test_a, eflags, (uint32*)cr3);
		task_add("testb", kernel_task_test_b, eflags, (uint32*)cr3);
		//task_add("testc", kernel_task_test_c, eflags, (uint32*)cr3);
		//*/

		//kassert(true == false);
		//safeptr<int> test(0x0);

		libk::printf("Value is %i!\n", util::parse<int32>("1234").val());
		libk::printf("Value is %i!\n", util::parse<int32>(" 	-47 ").val());
		libk::printf("Value is %i!\n", util::parse<int32>("0x80").val());
		libk::printf("Value is %i!\n", util::parse<int32>("-0b1010").val());
		libk::printf("Value is %i!\n", util::parse<int64>("  0x47A4a ").val());
		libk::printf("Value is %i!\n", util::parse<int16>("4765", 2).val());

		auto strtest1 = makestr("13");
		auto strtest2 = makestr("47");
		auto strtest3 = strtest1 + strtest2;
		libk::printf("String is '%s', size = %i!\n", strtest3.raw(), strtest3.len());
		libk::printf("Value is %s.\n", util::compose<int32>(util::parse<int32>(strtest3).val()).val());

		libk::printf((makestr("Number ") + util::compose<int32>(util::parse<int32>("650").val(), 16).val() + makestr(" haha!\n")).raw());

		// Enable the scheduler
		//task_enable_scheduler();

		kernel_welcome();

		while (prompt() == 0);
	}

	static void kernel_task_test_a()
	{
		while (true)
		{
			volatile uint32 a = 0;
			while (a < 5000) a ++;

			libk::printf("A");
		}
	}

	static void kernel_task_test_b()
	{
		while (true)
		{
			volatile uint32 a = 0;
			while (a < 5000) a ++;

			libk::printf("B");
		}
	}

	static void kernel_task_test_c()
	{
		while (true)
		{
			volatile uint32 a = 0;
			while (a < 5000) a ++;

			//libk::printf("C");
		}
	}
}
