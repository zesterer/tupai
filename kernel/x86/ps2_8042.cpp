//
// file : ps2_8042.cpp
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
#include <tupai/x86/ps2_8042.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/interrupt.hpp>
#include <tupai/debug.hpp>

namespace tupai
{
	namespace x86
	{
		static bool ps2_8042_initiated = false;
		static bool ps2_8042_disabled = false;
		static int  ps2_8042_channel = 0;
		static bool ps2_8042_channel_open[] = { false, };

		static const char* port_names[4] = {"PS20", "PS21"};

		static const uint16_t PS28042_PORT_CMD    = 0x64;
		static const uint16_t PS28042_PORT_STATUS = 0x64;
		static const uint16_t PS28042_PORT_DATA   = 0x60;

		static const uint8_t PS28042_CMD_ENABLE1  = 0xAE;
		static const uint8_t PS28042_CMD_ENABLE2  = 0xA8;
		static const uint8_t PS28042_CMD_DISABLE1 = 0xAD;
		static const uint8_t PS28042_CMD_DISABLE2 = 0xA7;

		static const uint8_t PS28042_CMD_SENDCMD2 = 0xD4;

		static const uint8_t PS28042_CMD_READBYTE0  = 0x20;
		static const uint8_t PS28042_CMD_WRITEBYTE0 = 0x60;

		static const uint8_t PS28042_CMD_SELFTEST = 0xAA;
		static const uint8_t PS28042_CMD_TEST1    = 0xAB;
		static const uint8_t PS28042_CMD_TEST2    = 0xA9;

		static const uint8_t PS2_RESET = 0xFF;

		static void ps2_8042_send_cmd(uint8_t code)
		{
			outb(PS28042_PORT_CMD, code);
			wait(150);
		}

		static void ps2_8042_write_data(uint8_t data)
		{
			outb(PS28042_PORT_DATA, data);
			wait(150);
		}

		static uint8_t ps2_8042_read_data()
		{
			wait(150);
			uint8_t val = inb(PS28042_PORT_DATA);
			return val;
		}

		static uint8_t ps2_8042_read_status()
		{
			wait(150);
			uint8_t val = inb(PS28042_PORT_STATUS);
			return val;
		}

		void ps2_8042_init()
		{
			if (ps2_8042_initiated)
				return;

			// TODO: Detect PS2 existence with ACPI

			bool int_enabled = interrupt_enabled();
			if (int_enabled)
				interrupt_enable(false); // Begin critical section

			// First, disable both PS2 ports
			ps2_8042_send_cmd(PS28042_CMD_DISABLE1);
			ps2_8042_send_cmd(PS28042_CMD_DISABLE2);

			// Flush the output buffer of the controller
			ps2_8042_read_data();

			// Set controller configuration byte
			ps2_8042_send_cmd(PS28042_CMD_READBYTE0);
			uint8_t cfg = ps2_8042_read_data(); // Read configuration byte
			cfg &= ~((1 << 0) | (1 << 1) | (1 << 6)); // Disable first & second port interrupts and port translation
			ps2_8042_send_cmd(PS28042_CMD_WRITEBYTE0);
			ps2_8042_write_data(cfg);// Rewrite configuration byte

			// Perform controller self-test
			ps2_8042_send_cmd(PS28042_CMD_SELFTEST);
			wait(100); // Long wait to give the controller time to self-test
			uint8_t result = ps2_8042_read_data();
			if (result != 0x55)
			{
				debug_println("Warning: 8042 PS/2 controller failed self-test and will be disabled.");
				ps2_8042_disabled = true;
			}

			// Determine channel count
			if (cfg & (1 << 5)) // We already know if it's enabled from the cfg byte
				ps2_8042_channel = (1 << 0);
			else
			{
				ps2_8042_send_cmd(PS28042_CMD_ENABLE2); // Attempt to enable second channel
				uint8_t cfg = ps2_8042_read_data(); // Read configuration byte

				if (cfg & (1 << 5))
					ps2_8042_channel = (1 << 0);
				else
					ps2_8042_channel = (1 << 0) | (1 << 1);
			}

			// Test performance
			ps2_8042_send_cmd(PS28042_CMD_TEST1); // Check first channel
			wait(150); // Long wait for test
			result = ps2_8042_read_data();
			if (result != 0x00)
			{
				debug_println("Warning: 8042 PS/2 channel 1 failed performance test with error '", result, "' and will be disabled.");
				ps2_8042_channel &= ~(1 << 0);
			}
			if (ps2_8042_channel | (1 << 1))
			{
				ps2_8042_send_cmd(PS28042_CMD_TEST2);  // Check second channel
				wait(150); // Long wait for test
				result = ps2_8042_read_data();

				if (result != 0x00)
				{
					debug_println("Warning: 8042 PS/2 channel 2 failed performance test with error '", result, "' and will be disabled.");
					ps2_8042_channel &= ~(1 << 1);
				}
			}

			// Enable channels & IRQs
			ps2_8042_send_cmd(PS28042_CMD_ENABLE1);
			ps2_8042_send_cmd(PS28042_CMD_ENABLE2);
			ps2_8042_send_cmd(PS28042_CMD_READBYTE0);
			cfg = ps2_8042_read_data(); // Read configuration byte
			cfg |= (1 << 0) | (1 << 1); // Enable first & second port interrupts
			ps2_8042_send_cmd(PS28042_CMD_WRITEBYTE0);
			ps2_8042_write_data(cfg);// Rewrite configuration byte

			// Reset PS/2 devices
			ps2_8042_write(0, PS2_RESET);
			ps2_8042_write(1, PS2_RESET);

			// Debug output
			if (ps2_8042_channel & (1 << 0)) debug_println("Detected 8042 PS/2 channel 1 device.");
			if (ps2_8042_channel & (1 << 0)) debug_println("Detected 8042 PS/2 channel 2 device.");
			debug_println("Finished 8042 PS/2 controller initiation");

			if (int_enabled)
				interrupt_enable(true); // End critical section

			ps2_8042_initiated = true;
		}

		size_t ps2_8042_count_ports()
		{
			return 2;
		}

		const char** ps2_8042_list_ports()
		{
			return port_names;
		}

		bool ps2_8042_open_port(int port_id)
		{
			if (!ps2_8042_initiated || !(ps2_8042_channel | (1 << port_id)) || ps2_8042_disabled || ps2_8042_channel_open[port_id])
				return false;

			ps2_8042_channel_open[port_id] = true;

			// Do nothing
			return false;
		}

		void ps2_8042_write(int port_id, uint8_t c)
		{
			if (!ps2_8042_initiated || !(ps2_8042_channel | (1 << port_id)) || ps2_8042_disabled)
				return;

			if (port_id == 0)
			{
				while (ps2_8042_read_status() & (1 << 1));
				ps2_8042_write_data(c);
			}
			else
			{
				ps2_8042_send_cmd(PS28042_CMD_SENDCMD2);
				while (ps2_8042_read_status() & (1 << 1));
				ps2_8042_write_data(c);
			}
		}

		uint8_t ps2_8042_read(int port_id)
		{
			if (!ps2_8042_initiated || !(ps2_8042_channel | (1 << port_id)) || ps2_8042_disabled)
				return 0x0;

			while ((ps2_8042_read_status() & 0x01) == 0);
			return ps2_8042_read_data();
		}
	}
}
