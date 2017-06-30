//
// file : arch.cpp
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
#include <tupai/arch.hpp>
#include <tupai/x86/pic.hpp>
#include <tupai/x86/pit.hpp>
#include <tupai/x86/ps2_8042.hpp>
#include <tupai/x86/pci.hpp>
#include <tupai/x86/kbd.hpp>

namespace tupai
{
	const char* arch_get_family()
	{
		return "x86";
	}

	void arch_init()
	{
		// Initiate the PIC
		x86::pic_bind();
		x86::pic_init();

		// Initiate the PIT
		x86::pit_bind();
		x86::pit_init();

		// Initiate PS2
		x86::ps2_8042_init();

		// Initiate the PCI bus
		x86::pci_init();

		// Initiate the Keyboard
		x86::kbd_bind();
		x86::kbd_init();
	}
}
