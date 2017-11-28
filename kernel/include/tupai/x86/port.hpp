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

namespace tupai::x86::io
{
	template<size_t SIZE> inline void out(uint16_t port, uint32_t val, bool do_wait = true);
	template<size_t SIZE> inline uint32_t in(uint16_t port, bool do_wait = true);

	inline void wait()
	{
		asm volatile ( "outb %%al, $0x80" : : "a"(0) ); // Port 0x80 is free
	}

	template<> inline void out<8>(uint16_t port, uint32_t val, bool do_wait)
	{
		asm volatile ("outb %0, %1" :: "a"(static_cast<uint8_t>(val)), "Nd"(port));

		if (do_wait)
			wait();
	}

	template<> inline void out<16>(uint16_t port, uint32_t val, bool do_wait)
	{
		asm volatile ("outw %0, %1" :: "a"(static_cast<uint16_t>(val)), "Nd"(port));

		if (do_wait)
			wait();
	}

	template<> inline void out<32>(uint16_t port, uint32_t val, bool do_wait)
	{
		asm volatile ("outl %0, %1" :: "a"(static_cast<uint32_t>(val)), "Nd"(port));

		if (do_wait)
			wait();
	}

	template<> inline uint32_t in<8>(uint16_t port, bool do_wait)
	{
		uint8_t val;
		asm volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));

		if (do_wait)
			wait();

		return val;
	}

	template<> inline uint32_t in<16>(uint16_t port, bool do_wait)
	{
		uint16_t val;
		asm volatile ("inw %1, %0" : "=a"(val) : "Nd"(port));

		if (do_wait)
			wait();

		return val;
	}

	template<> inline uint32_t in<32>(uint16_t port, bool do_wait)
	{
		uint32_t val;
		asm volatile ("inl %1, %0" : "=a"(val) : "Nd"(port));

		if (do_wait)
			wait();

		return val;
	}
}

#endif
