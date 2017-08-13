//
// file : ps2.cpp
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
#include <tupai/dev/ps2.hpp>
#include <tupai/util/str.hpp>

#if defined(ARCH_FAMILY_X86)
	#include <tupai/x86/ps2_8042.hpp>
#else
	#warning "Architecture provides no PS/2 device!"
#endif

namespace tupai
{
	namespace dev
	{
		static bool ps2_initiated = false;

		static const uint8_t PS2_CMD_DISABLE_SCANNING = 0xF5;
		static const uint8_t PS2_CMD_IDENTIFY = 0xF2;

		static const uint8_t PS2_ACK = 0xFA;

		void ps2_init()
		{
			if (ps2_initiated)
				return;

			#if defined(ARCH_FAMILY_X86)
				x86::ps2_8042_init();
			#endif

			ps2_initiated = true;
		}

		size_t ps2_count_ports()
		{
			#if defined(ARCH_FAMILY_X86)
				return x86::ps2_8042_count_ports();
			#else
				return 0;
			#endif
		}

		const char** ps2_list_ports()
		{
			#if defined(ARCH_FAMILY_X86)
				return x86::ps2_8042_list_ports();
			#endif
		}

		int ps2_open_port(const char* port)
		{
			const char** port_names = ps2_list_ports();
			size_t port_count = ps2_count_ports();
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
			bool success = false;
			#if defined(ARCH_FAMILY_X86)
				success = x86::ps2_8042_open_port(port_id);
			#endif

			if (!success)
				return -1;

			return port_id;
		}

		void ps2_write(int port_id, uint8_t val)
		{
			if (port_id == -1) // Invalid port
				return;

			#if defined(ARCH_FAMILY_X86)
				x86::ps2_8042_write(port_id, val);
			#endif
		}

		uint8_t ps2_read(int port_id)
		{
			if (port_id == -1) // Invalid port, just return null data
				return 0;

			#if defined(ARCH_FAMILY_X86)
				return x86::ps2_8042_read(port_id);
			#else
				return 0;
			#endif
		}

		ps2_device_type ps2_get_device_type(int port_id)
		{
			uint8_t response;

			// Disable scanning
			ps2_write(port_id, PS2_CMD_DISABLE_SCANNING);
			response = ps2_read(port_id);
			if (response != PS2_ACK)
				return ps2_device_type::UNKNOWN;

			// Identify device
			ps2_write(port_id, PS2_CMD_IDENTIFY);
			response = ps2_read(port_id);
			if (response != PS2_ACK)
				return ps2_device_type::UNKNOWN;

			// Retrieve ID bytes
			uint8_t id_bytes[2];
			id_bytes[0] = ps2_read(port_id);
			id_bytes[1] = ps2_read(port_id);

			// Determine device
			if (id_bytes[0] == 0x00)
				return ps2_device_type::MOUSE_NOSCROLL;
			else if (id_bytes[0] == 0x03)
				return ps2_device_type::MOUSE_SCROLL;
			else if (id_bytes[0] == 0x04)
				return ps2_device_type::MOUSE_5BUTTON;
			else if (id_bytes[0] == 0xAB)
			{
				if (id_bytes[1] == 0x41 || id_bytes[1] == 0xC1)
					return ps2_device_type::KEYBOARD_MF2_TRANSLATION;
				else if (id_bytes[1] == 0x83)
					return ps2_device_type::KEYBOARD_MF2;
				else
					return ps2_device_type::UNKNOWN;
			}
			else
				return ps2_device_type::UNKNOWN;
		}
	}
}
