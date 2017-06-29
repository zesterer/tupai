//
// file : pci.cpp
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
#include <tupai/x86/pci.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/debug.hpp>

namespace tupai
{
	namespace x86
	{
		static const uint16_t PCI_ADDR_PORT = 0xCF8;
		static const uint16_t PCI_DATA_PORT = 0xCFC;

		uint16_t pci_cfg_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
		{
			uint32_t lbus = (uint32_t)bus;
			uint32_t lslot = (uint32_t)slot;
			uint32_t lfunc = (uint32_t)func;

			// Assemble PCI address
			uint32_t addr = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

			// Write address
			outl(PCI_ADDR_PORT, addr);

			return (uint16_t)((inl(PCI_DATA_PORT) >> ((offset & 0x2) * 8)) & 0xFFFF);
		}

		void pci_init()
		{
			debug_println("PCI bus initiated!");
		}
	}
}
