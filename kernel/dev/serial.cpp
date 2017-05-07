//
// file : serial.cpp
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
#include <tupai/dev/serial.hpp>
#include <tupai/util/str.hpp>

#if defined(ARCH_FAMILY_x86)
	#include <tupai/x86/serial.hpp>
#elif defined(ARCH_rpi2)
	#include <tupai/arm/rpi2/serial.hpp>
#else
	#warning "Architecture provides no serial device!"
#endif

namespace tupai
{
	namespace dev
	{
		bool serial_initiated = false;

		void serial_init()
		{
			if (serial_initiated)
				return;

			#if defined(ARCH_FAMILY_x86)
				x86::serial_init();
			#elif defined(ARCH_rpi2)
				arm::rpi2::serial_init();
			#endif

			serial_initiated = true;
		}

		size_t serial_count_ports()
		{
			#if defined(ARCH_FAMILY_x86)
				return x86::serial_count_ports();
			#elif defined(ARCH_rpi2)
				return arm::rpi2::serial_count_ports();
			#endif
		}

		const char** serial_list_ports()
		{
			#if defined(ARCH_FAMILY_x86)
				return x86::serial_list_ports();
			#elif defined(ARCH_rpi2)
				return arm::rpi2::serial_list_ports();
			#endif
		}

		int serial_open_port(const char* port, uint32_t baudrate, uint8_t databits, uint8_t stopbits, serial_parity parity)
		{
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

			if (port_id == -1) // The port name was not found
				return port_id;

			// It's valid, so attempt to open a port
			bool success;
			#if defined(ARCH_FAMILY_x86)
				success = x86::serial_open_port(port_id, baudrate, databits, stopbits, parity);
			#elif defined(ARCH_rpi2)
				success = arm::rpi2::serial_open_port(port_id, baudrate, databits, stopbits, parity);
			#endif

			if (!success)
				return -1;

			return port_id;
		}

		void serial_write(int port_id, uint8_t val)
		{
			if (port_id == -1) // Invalid port
				return;

			#if defined(ARCH_FAMILY_x86)
				x86::serial_write(port_id, val);
			#elif defined(ARCH_rpi2)
				arm::rpi2::serial_write(port_id, val);
			#endif
		}

		uint8_t serial_read(int port_id)
		{
			if (port_id == -1) // Invalid port, just return null data
				return 0;

			#if defined(ARCH_FAMILY_x86)
				return x86::serial_read(port_id);
			#elif defined(ARCH_rpi2)
				return arm::rpi2::serial_read(port_id);
			#endif
		}
	}
}
