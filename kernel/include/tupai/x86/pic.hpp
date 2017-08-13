//
// file : pic.hpp
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

#ifndef TUPAI_X86_PIC_HPP
#define TUPAI_X86_PIC_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		const uint8_t PIC_REMAP_OFFSET = 32;

		void     pic_bind();
		void     pic_init();
		uint16_t pic_get_irr();
		uint16_t pic_get_isr();
		void     pic_ack(uint8_t irq);
		void     pic_mask(uint8_t irq, bool enabled);
	}
}

#endif
