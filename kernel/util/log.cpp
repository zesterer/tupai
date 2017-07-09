//
// file : log.cpp
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
#include <tupai/util/log.hpp>
#include <tupai/util/hwlock.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/dev/tty.hpp>

namespace tupai
{
	namespace util
	{
		typedef vector_t<char> log_t;

		vector_t<log_t> logs;
		hwlock_t        hwlock;
		log_t           clog;

		void __log_ostream::end()
		{
			if (clog.size() > 0)
			{
				logs.push(clog);
				clog = log_t();
			}
		}

		void __log_ostream::write(char c)
		{
			hwlock.lock(); // Begin critical section

			clog.push(c);
			dev::tty_write(c);

			hwlock.unlock(); // End critical section
		}
	}
}
