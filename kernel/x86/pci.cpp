//
// file : pci.cpp
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
