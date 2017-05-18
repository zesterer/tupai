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
// but WITHOUT ANY WARRANTY// without even the implied warranty of
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
#include <tupai/sys/thread.hpp>
#include <tupai/sys/pipe.hpp>
#include <tupai/debug.hpp>
#include <tupai/util/out.hpp>

#if defined(ARCH_FAMILY_x86)
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

		static volatile util::mutex tty_mutex;

		// The I/O pipe. 256 character buffer
		static volatile sys::pipe<2048> iopipe;

		static void tty_in_thread();
		static void tty_out_thread();

		void tty_init()
		{
			tty_mutex.lock(); // Begin critical section

			if (tty_initiated)
			{
				tty_mutex.unlock(); // End critical section
				return;
			}

			#if defined(ARCH_FAMILY_x86)
				x86::textmode_init();
			#endif

			// Open a serial port
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
						"  baudrate -> ", 57600, '\n',
						"  databits -> ", 8, '\n',
						"  stopbits -> ", 1, '\n',
						"  parity   -> ", "NONE", '\n'
					);
				}
				else
					debug_print("Could not find port for serial tty output!\n");
			}

			// Create the TTY I/O threads
			if (sys::threading_enabled())
			{
				sys::thread_create(tty_in_thread);
				sys::thread_create(tty_out_thread);
			}
			else
			{
				tty_mutex.unlock(); // End critical section
				return;
			}

			tty_initiated = true;

			tty_mutex.unlock(); // End critical section
		}

		void tty_write(char c)
		{
			if (tty_initiated) tty_mutex.lock(); // Begin critical section
			iopipe.write(c);
			if (tty_initiated) tty_mutex.unlock(); // End critical section
		}

		void tty_print(const char* str)
		{
			for (size_t i = 0; str[i] != '\0'; i ++)
				tty_write(str[i]);
		}

		char tty_read()
		{
			if (tty_initiated) tty_mutex.lock(); // Begin critical section
			char val = iopipe.read();
			if (tty_initiated) tty_mutex.unlock(); // End critical section
			return val;
		}

		void tty_in_thread()
		{
			while (true)
			{
				unsigned char c = dev::serial_read(tty_serial_port);
				iopipe.write_in(c);
			}
		}

		void tty_out_thread()
		{
			while (true)
			{
				unsigned char c = iopipe.read_out();

				#if defined(ARCH_FAMILY_x86)
					x86::textmode_write(c);
				#endif

				dev::serial_write(tty_serial_port, c);

				if (c == '\n')
					dev::serial_write(tty_serial_port, '\r');
				//else if (c == '\r')
				//	dev::serial_write(tty_serial_port, '\n');
			}
		}
	}
}
