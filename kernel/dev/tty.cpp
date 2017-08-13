//
// file : tty.cpp
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
#include <tupai/dev/tty.hpp>
#include <tupai/dev/serial.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/hwlock.hpp>
#include <tupai/util/ansi.hpp>
#include <tupai/sys/pipe.hpp>
#include <tupai/task/task.hpp>
#include <tupai/debug.hpp>

#if defined(ARCH_FAMILY_X86)
	#include <tupai/x86/textmode.hpp>
#endif

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace dev
	{
		static volatile bool tty_initiated = false;
		static volatile int tty_serial_port = -1;

		static util::spinlock_t spinlock;

		// Unions are used here SPECIFICALLY to avoid global construction!
		union In  { sys::pipe_t pipe;  In() {} } in;
		union Out { sys::pipe_t pipe; Out() {} } out;

		static void tty_out_thread(int argc, char* argv[]);

		void tty_construct()
		{
			in.pipe  = sys::pipe_t();
			out.pipe = sys::pipe_t();
		}

		void tty_init()
		{
			spinlock.lock(); // Begin critical section

			if (!tty_initiated)
			{
				// Mount the pipes
				sys::mount_pipe(&in.pipe, "/dev/stdin");
				sys::mount_pipe(&out.pipe, "/dev/stdout");

				#if defined(ARCH_FAMILY_X86)
					x86::textmode_init();
					x86::textmode_cursor_enable();
				#endif

				// Open a serial port
				if (false)
				{
					// Find the names of available serial ports
					const char** serial_port_names = dev::serial_list_ports();
					// Search the serial port list, trying to open a debugging port
					for (size_t i = 0; i < dev::serial_count_ports() && tty_serial_port == -1; i ++)
						tty_serial_port = dev::serial_open_port(serial_port_names[i], 57600, 8, 1, dev::serial_parity::NONE);

					if (tty_serial_port != -1)
					{
						debug_print(
							"Started serial tty output on ", serial_port_names[tty_serial_port], '\n',
							"  baudrate -> ", (uint16_t)57600, '\n',
							"  databits -> ", (uint16_t)8, '\n',
							"  stopbits -> ", (uint16_t)1, '\n',
							"  parity   -> ", "NONE", '\n'
						);
					}
					else
						debug_print("Could not find port for serial tty output!\n");
				}

				tty_initiated = true;
			}

			spinlock.unlock(); // End critical section

			// Create the TTY I/O thread
			task::get_current().spawn_thread(tty_out_thread);
		}

		void tty_write(char c)
		{
			spinlock.lock(); // Begin critical section
			out.pipe.write_unsafe(c);
			spinlock.unlock(); // End critical section
		}

		void tty_print(const char* str)
		{
			for (size_t i = 0; str[i] != '\0'; i ++)
				tty_write(str[i]);
		}

		char tty_read()
		{
			spinlock.lock(); // Begin critical section
			char val = in.pipe.read_unsafe();
			spinlock.unlock(); // End critical section
			return val;
		}

		void tty_write_in(char c)
		{
			util::hwlock_acquire(); // Begin critical section
			in.pipe.write_unsafe(c);
			util::hwlock_release(); // End critical section
		}

		void tty_out_thread(int argc, char* argv[])
		{
			(void)argc;
			(void)argv;

			util::ansi_t ansi;

			while (true)
			{
				unsigned char c = out.pipe.read(); // Unsafe call to avoid exception locking

				util::ansi_cmd_t cmd = ansi.consume(c);

				#if defined(ARCH_FAMILY_X86)
					x86::textmode_apply(cmd);
				#endif

				if (cmd.type == util::ansi_cmd::PASS)
				{
					dev::serial_write(tty_serial_port, cmd.c);

					if (cmd.c == '\n')
						dev::serial_write(tty_serial_port, '\r');
				}
			}
		}
	}
}
