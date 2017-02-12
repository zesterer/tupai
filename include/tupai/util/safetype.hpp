/*
* 	file : safetype.hpp
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

#ifndef TUPAI_UTIL_SAFEPTR_HPP
#define TUPAI_UTIL_SAFEPTR_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/kdebug.hpp>

namespace tupai
{
	template <typename T>
	struct safeptr
	{
	private:
		T* ptr;
		bool valid = false;

		void init(T* ptr, bool valid)
		{
			this->ptr = ptr;
			this->valid = valid;

			// TODO : fix this. It creates infinite loop if VGA is not enabled
			//if (!this->is_valid())
			//	klog("safeptr initialised with an invalid pointer", klog_level::WARNING);
		}

	public:
		safeptr() { this->init(nullptr, false); }
		safeptr(T* ptr, bool valid = true) { this->init(ptr, valid); }

		bool is_null() { return this->ptr == nullptr; }
		bool is_valid()
		{
			// TODO: Add proper checks to make sure it resides within data segments
			return !this->is_null() && this->valid;
		}

		T& deref()
		{
			if (this->is_valid())
				return *this->ptr;
			else
				kpanic("safeptr attempted to dereference an invalid pointer");
		}

		T* val()
		{
			if (this->is_valid())
				return this->ptr;
			else
				kpanic("safeptr attempted to evaluate an invalid pointer");
		}
	};

	template <typename T>
	struct safeval
	{
	private:
		T value;
		bool valid = false;

		void init(T value, bool valid)
		{
			this->value = value;
			this->valid = valid;

			if (!this->is_valid())
				klog("safeval initialised with an invalid value", klog_level::WARNING);
		}

	public:
		safeval(T ptr, bool valid = true) { this->init(ptr, valid); }

		bool is_valid()
		{
			// TODO: Add proper checks to make sure it resides within data segments
			return this->valid;
		}

		T val()
		{
			if (this->is_valid())
				return this->value;
			else
				kpanic("safeval attempted to realise an invalid value");
		}
	};
}

#endif
