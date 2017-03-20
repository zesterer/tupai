/*
* 	file : common.hpp
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

#ifndef TUPAI_FS_COMMON_HPP
#define TUPAI_FS_COMMON_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/util/mem.hpp>
#include <tupai/util/cstr.hpp>
#include <tupai/util/math.hpp>
#include <tupai/util/string.hpp>
#include <tupai/util/vector.hpp>

namespace tupai
{
	namespace fs
	{
		static const umem PATH_MAX_LENGTH = 512;
		static const umem NODENAME_MAX__LENGTH = 256;

		struct path
		{
		private:
			util::vector<util::string> pathvec;
			bool absolute = true;
			bool valid = true;

			void validate()
			{
				for (umem i = 0; i < this->pathvec.length(); i ++)
				{
					if (this->pathvec[i].length() <= 0)
					{
						this->valid = false;
						return;
					}
				}

				this->valid = true;
			}

			void set(util::string pathstr)
			{
				this->pathvec = pathstr.split('/');
				this->absolute = (pathstr[0] == '/');

				this->validate();
			}

		public:
			path()
			{
				this->set("/");
			}

			path(util::string pathstr)
			{
				this->set(pathstr);
			}

			const util::string& getAt(umem i) const
			{
				return this->pathvec.getAt(i);
			}

			util::string& getAtMut(umem i)
			{
				return this->pathvec.getAtMut(i);
			}

			util::string& operator[](umem i)
			{
				return this->getAtMut(i);
			}

			util::vector<util::string>& vec()
			{
				return this->pathvec;
			}

			umem depth() const
			{
				return this->pathvec.length();
			}

			bool is_valid() const
			{
				return this->valid;
			}

			bool is_absolute() const
			{
				return this->absolute;
			}

			bool append(path npath)
			{
				if (npath.is_absolute() && npath.is_valid())
					return false;
				else
				{
					for (umem i = 0; i < npath.depth(); i ++)
						this->pathvec.push(npath[i]);

					return true;
				}
			}

			bool append(util::string pathstr)
			{
				return this->append(path(pathstr));
			}

			util::string str() const
			{
				util::string nstr;
				for (umem i = 0; i < this->pathvec.length(); i ++)
					nstr += "/" + this->pathvec.getAt(i);
				return nstr;
			}

			bool equals(const path& other) const
			{
				if (this->pathvec.length() != other.depth())
					return false;

				for (umem i = 0; i < this->pathvec.length(); i ++)
				{
					if (this->pathvec.getAt(i) != other.getAt(i))
						return false;
				}

				return true;
			}
		};

		bool     path_is_relative(const char* path);
		status_t path_concat(const char* path0, const char* path1, char* buffer);
	}
}

#endif
