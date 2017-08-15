//
// file : serial.cpp
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
#include <tupai/dev/serial.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/spinlock.hpp>

#if defined(ARCH_FAMILY_X86)
	#include <tupai/x86/serial.hpp>
#elif defined(ARCH_RPI2)
	#include <tupai/arm/rpi2/serial.hpp>
#else
	#warning "Architecture provides no serial device!"
#endif

namespace tupai
{
	namespace dev
	{
		static bool serial_initiated = false;
		static util::spinlock_t spinlock;

		void serial_init()
		{
			spinlock.lock(); // Begin critical section

			if (!serial_initiated)
			{
				#if defined(ARCH_FAMILY_X86)
					x86::serial_init();
				#elif defined(ARCH_RPI2)
					arm::rpi2::serial_init();
				#endif

				serial_initiated = true;
			}

			spinlock.unlock(); // End critical section
		}

		size_t serial_count_ports()
		{
			spinlock.lock(); // Begin critical section

			#if defined(ARCH_FAMILY_X86)
				size_t val = x86::serial_count_ports();
			#elif defined(ARCH_RPI2)
				size_t val = arm::rpi2::serial_count_ports();
			#endif

			spinlock.unlock(); // End critical section
			return val;
		}

		const char** serial_list_ports()
		{
			spinlock.lock(); // Begin critical section

			#if defined(ARCH_FAMILY_X86)
				const char** val = x86::serial_list_ports();
			#elif defined(ARCH_RPI2)
				const char** val = arm::rpi2::serial_list_ports();
			#endif

			spinlock.unlock(); // End critical section
			return val;
		}

		int serial_open_port(const char* port, uint32_t baudrate, uint8_t databits, uint8_t stopbits, serial_parity parity)
		{
			spinlock.lock(); // Begin critical section

			const char** port_names = serial_list_ports();
			size_t port_count = serial_count_ports();
			int port_id = -1;

			// Search for the correct port name
			for (size_t i = 0; i < port_count; i ++)
			{
				if (util::str_equal(port_names[i], port))
				{
					port_id = i;
					break;
				}
			}

			bool success = false;
			if (port_id != -1) // The port name was found
			{
				// It's valid, so attempt to open a port
				#if defined(ARCH_FAMILY_X86)
					success = x86::serial_open_port(port_id, baudrate, databits, stopbits, parity);
				#elif defined(ARCH_RPI2)
					success = arm::rpi2::serial_open_port(port_id, baudrate, databits, stopbits, parity);
				#endif
			}

			spinlock.unlock(); // End critical section

			if (success)
				return port_id;
			else
				return -1;
		}

		void serial_write(int port_id, uint8_t val)
		{
			spinlock.lock(); // Begin critical section

			if (port_id != -1) // Invalid port
			{
				#if defined(ARCH_FAMILY_X86)
					x86::serial_write(port_id, val);
				#elif defined(ARCH_RPI2)
					arm::rpi2::serial_write(port_id, val);
				#endif
			}

			spinlock.unlock(); // End critical section
		}

		uint8_t serial_read(int port_id)
		{
			spinlock.lock(); // Begin critical section

			uint8_t val = 0;
			if (port_id != -1) // Invalid port, just return null data
			{
				#if defined(ARCH_FAMILY_X86)
					val = x86::serial_read(port_id);
				#elif defined(ARCH_RPI2)
					val = arm::rpi2::serial_read(port_id);
				#endif
			}

			spinlock.unlock(); // End critical section
			return val;
		}
	}
}
