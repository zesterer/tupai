//
// file : port.hpp
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

#ifndef TUPAI_X86_PORT_HPP
#define TUPAI_X86_PORT_HPP

#include <tupai/util/type.hpp>

namespace tupai::x86::vga
{
	static inline void out8(uint16_t port, uint8_t value)
	{
		asm volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
	}

	static inline void out16(uint16_t port, uint16_t value)
	{
		asm volatile ("outw %0, %1" :: "a"(value), "Nd"(port));
	}

	static inline void out32(uint16_t port, uint32_t value)
	{
		asm volatile ("outl %0, %1" :: "a"(value), "Nd"(port));
	}

	static inline uint8_t in8(uint16_t port)
	{
		uint8_t value;
		asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
		return value;
	}

	static inline uint16_t in16(uint16_t port)
	{
		uint16_t value;
		asm volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
		return value;
	}

	static inline uint32_t in32(uint16_t port)
	{
		uint32_t value;
		asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
		return value;
	}
}

#endif
