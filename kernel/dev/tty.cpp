//
// file : tty.cpp
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
		union In  { sys::pipe::pipe_t pipe;  In() {} } in;
		union Out { sys::pipe::pipe_t pipe; Out() {} } out;

		static void tty_out_thread(int argc, char* argv[]);

		void tty_construct()
		{
			in.pipe  = sys::pipe::pipe_t();
			out.pipe = sys::pipe::pipe_t();
		}

		void tty_init()
		{
			spinlock.lock(); // Begin critical section

			if (!tty_initiated)
			{
				// Mount the pipes
				sys::pipe::mount(&in.pipe, "/dev/stdin", true);
				sys::pipe::mount(&out.pipe, "/dev/stdout", true);

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
