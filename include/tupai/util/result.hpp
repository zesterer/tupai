//
// file : result.hpp
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

#ifndef TUPAI_UTIL_RESULT_HPP
#define TUPAI_UTIL_RESULT_HPP

// Tupai
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename Val, typename Err>
		class result
		{
		private:
			bool _is_err;
			union {
				Val _val;
				Err _err;
			};

		public:
			result(Val val)
			{
				this->_is_err = false;
				this->_val = val;
			}

			result(Err err)
			{
				this->_is_err = true;
				this->_err = err;
			}

			bool failed() { return this->_is_err; }
			Val& get_value() { if (this->_is_err) panic("Attempted to realise an erroneous value!"); return this->_val; }
			Err& get_error() { if (!this->_is_err) panic("No error occured, but code believes it did!"); return this->_err; }
		};
	}
}

#endif
