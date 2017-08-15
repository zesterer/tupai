//
// file : serial.hpp
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

#ifndef TUPAI_DEV_SERIAL_HPP
#define TUPAI_DEV_SERIAL_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace dev
	{
		enum class serial_parity : int
		{
			NONE  = 0,
			ODD   = 1,
			EVEN  = 2,
			MARK  = 3,
			SPACE = 4,
		};

		void serial_init();

		size_t       serial_count_ports();
		const char** serial_list_ports();

		int serial_open_port(const char* port, uint32_t baudrate = 57600, uint8_t databits = 8, uint8_t stopbits = 1, dev::serial_parity parity = serial_parity::NONE);

		void    serial_write(int port_id, uint8_t val);
		uint8_t serial_read(int port_id);
	}
}

#endif
