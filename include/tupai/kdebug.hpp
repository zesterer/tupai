/*
* 	file : kdebug.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_KDEBUG_HPP
#define TUPAI_KDEBUG_HPP

// Tupai
#include <tupai/type.hpp>

#define KBREAK() asm volatile ("xchgw %bx, %bx")
#define kassert(expr) tupai::__kassert__(expr, #expr, __LINE__, __func__, __FILE__)

namespace tupai
{
	enum class klog_level
	{
		INFO,
		WARNING,
		ERROR,
		PANIC,
	};

	void klog(const char* msg, klog_level level = klog_level::INFO);
	void klog_init(const char* msg, bool success = true);
	void kpanic(const char* msg, uint32 error = 0x0) __attribute__((noreturn));
	bool __kassert__(bool expr, const char* expr_str, int line, const char* func, const char* file);
}

#endif
