//
// file : typename.hpp
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

#ifndef TUPAI_UTIL_TYPEDATA_HPP
#define TUPAI_UTIL_TYPEDATA_HPP

#include <tupai/util/def/typedata.hpp>
#include <tupai/util/refstr.hpp>

namespace tupai::util
{
	template <typename T>
	constexpr GenRefStr<char> type_name()
	{
		return GenRefStr<char>(__PRETTY_FUNCTION__ + 74, cstr_len(__PRETTY_FUNCTION__) - 75); // TODO : This is a total hack. Maintain it well, and replace it when the C++ standard matures
	}
}

#endif
