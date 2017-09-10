//
// file : str.hpp
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

#ifndef TUPAI_UTIL_STR_HPP
#define TUPAI_UTIL_STR_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static constexpr size_t str_len(const char* str)
		{
			size_t i = 0;
			for (; str[i] != '\0'; i ++);
			return i;
		}

		static constexpr void str_cpy(const char* src, char* dest)
		{
			size_t i = 0;
			for (; src[i] != '\0'; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		static constexpr void str_cpy_n(const char* src, char* dest, size_t n)
		{
			size_t i = 0;
			for (; src[i] != '\0' && i + 1 < n; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		template <size_t N>
		static constexpr void str_cpy_n(const char* src, char(&dest)[N])
		{
			size_t i = 0;
			for (; src[i] != '\0' && i + 1 < N; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		static constexpr bool str_equal(const char* str0, const char* str1)
		{
			size_t i = 0;
			do
			{
				if (str0[i] != str1[i])
					return false;
				i ++;
			} while (str0[i] != '\0' || str1[i] != '\0');

			return true;
		}

		static constexpr unsigned long str_hash(const char* str) // djb2
		{
			unsigned long hash = 5381;

			for (size_t i = 0; str[i] != '\0'; i ++)
				hash = ((hash << 5) + hash) + str[i];

			return hash;
		}

		template <typename T>
		class BasicStr
		{
			private:
				T* _data = nullptr;
				size_t _len= 0;

				static size_t strLength(const T* data)
				{
					size_t i;
					for (i = 0; data[i] != '\0'; i ++);
					return i;
				}

				static void strCopy(const T* src, T* dest)
				{
					size_t i = 0;
					do { dest[i] = src[i]; } while (src[i++] != '\0');
				}

				BasicStr<T>& _copy(const BasicStr<T>& other)
				{
					this->_len = other._len;
					this->_data = new T[this->_len + 1];
					BasicStr<T>::strCopy(other._data, this->_data);
					return *this;
				}

				BasicStr<T>& _move(BasicStr<T>& other)
				{
					this->_len = other._len;
					this->_data = other._data;
					other._len = 0;
					this->_data = nullptr;
					return *this;
				}

			public:
				BasicStr() {}

				BasicStr(const T* data)
				{
					this->_len = BasicStr<T>::strLength(data);
					this->_data = new T[this->_len + 1];
					BasicStr<T>::strCopy(data, this->_data);
				}

				BasicStr(const BasicStr<T>& other)
				{
					this->_copy(other);
				}

				BasicStr<T>& operator=(const BasicStr<T>& other)
				{
					return this->_copy(other);
				}

				BasicStr(BasicStr<T>&& other)
				{
					this->_move(other);
				}

				BasicStr<T>& operator=(BasicStr<T>&& other)
				{
					return this->_move(other);
				}

				~BasicStr()
				{
					if (this->_data != nullptr)
						delete this->_data;
				}
		};

		typedef BasicStr<char> Str;
	}
}

#endif
