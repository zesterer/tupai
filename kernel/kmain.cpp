//
// file : kmain.cpp
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

#include <tupai/util/boot.hpp>
#include <tupai/util/log.hpp>
#include <tupai/cpu/cpu.hpp>

#include <tupai/util/vec.hpp>
#include <tupai/util/str.hpp>

namespace tupai
{
	extern "C" void kmain()
	{
		util::bootlog("Entered kernel main");

		auto arr = util::make_arr("kernel");
		util::RefStr str1 = util::RefStr::from(arr);

		util::logln("Hello, {} world!", str1);

		auto sarr = util::make_arr({"hello", "there"});
		util::logln("arr = {}", sarr);

		cpu::wait();
	}
}
