//
// file : ref.hpp
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

#ifndef TUPAI_UTIL_REF_HPP
#define TUPAI_UTIL_REF_HPP

// Tupai
#include <tupai/util/hashtable.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/type.hpp>
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		struct thread_mutex { void lock() {} void unlock() {} }; // TODO : Replace this

		template <typename T>
		class _BaseRef;

		template <typename T>
		class Ref;

		template <typename T>
		class WRef;

		template<typename T>
		class _BaseRef
		{
			friend class Ref<T>;
			friend class WRef<T>;

			protected:
				struct _Count
				{
					T* item;
					size_t strong;
					size_t weak;
					thread_mutex lock;
					thread_mutex item_lock;

					template <typename ... Args>
					_Count(Args ... args) : item(new T(args ...)), strong(0), weak(0) {}

					bool isValid()
					{
						this->lock.lock(); // Begin critical section
						bool valid = this->strong > 0;
						this->lock.unlock(); // End critical section

						return valid;
					}

					bool validateAndLock()
					{
						this->lock.lock(); // Begin critical section
						bool valid = this->strong > 0;
						this->lock.unlock(); // End critical section

						if (valid)
							this->lockItem();

						return valid;
					}

					void incStrong()
					{
						this->lock.lock(); // Begin critical section

						this->strong ++;

						this->lock.unlock(); // End critical section

						this->incWeak(); // Strong references imply weak references too
					}

					void decStrong()
					{
						this->lock.lock(); // Begin critical section

						this->strong --;

						if (this->strong == 0) // Delete if strong references fall to 0
						{
							delete this->item;
						}

						this->lock.unlock(); // End critical section

						this->decWeak(); // Strong references imply weak references too
					}

					void incWeak()
					{
						this->lock.lock(); // Begin critical section

						this->weak ++;

						this->lock.unlock(); // End critical section
					}

					void decWeak()
					{
						this->lock.lock(); // Begin critical section

						this->weak --;

						if (this->weak == 0) // Delete if strong references fall to 0
						{
							this->lock.unlock(); // End critical section (we're the last reference)

							delete this;
						}
						else
							this->lock.unlock(); // End critical section

						this->lock.unlock(); // End critical section
					}

					bool lockItem()
					{
						this->item_lock.lock();

						return true;
					}

					bool unlockItem()
					{
						this->item_lock.unlock();
						return true;
					}
				};

			public:
				class _Proxy
				{
					private:
						_Count* _count;

					public:
						_Proxy(_Count* count) : _count(count)
						{
							this->_count->lockItem();
						}

						T* operator->()
						{
							return this->_count->item;
						}

						~_Proxy()
						{
							this->_count->unlockItem();
						}
				};

			protected:
				_Count* _count;

				_BaseRef() : _count(nullptr) {}

				template <typename ... Args>
				_BaseRef(Args ... args) : _count(new _Count(args ...)) {}

			public:
				bool isValid()
				{
					if (this->_count == nullptr)
						return false;
					else
						return this->_count->isValid();
				}

				bool lock()
				{
					if (this->_count == nullptr)
						return false;
					else
						return this->_count->lockItem();
				}

				bool unlock()
				{
					if (this->_count == nullptr)
						return false;
					else
						return this->_count->unlockItem();
				}
		};

		template <typename T>
		class Ref : public _BaseRef<T>
		{
			private:
				Ref<T>& _copy(const Ref<T>& other)
				{
					if (this->_count != nullptr)
						this->_count->decStrong();

					this->_count = other._count;
					this->_count->incStrong();

					return *this;
				}

				Ref<T>& _move(Ref<T>& other)
				{
					if (this->_count != nullptr)
						this->_count->decStrong();

					this->_count = other._count;
					other._count = nullptr;

					return *this;
				}

			public:
				template <typename ... Args>
				Ref(Args ... args) : _BaseRef<T>(args ...)
				{
					this->_count->incStrong();
				}

				Ref(const Ref<T>& other) : _BaseRef<T>()
				{
					this->_copy(other);
				}

				Ref<T>& operator=(const Ref<T>& other)
				{
					return this->_copy(other);
				}

				Ref(Ref<T>&& other) : _BaseRef<T>()
				{
					this->_move(other);
				}

				Ref<T>& operator=(Ref<T>&& other)
				{
					return this->_move(other);
				}

				~Ref()
				{
					if (this->_count != nullptr) // If _count is null, this is a moved reference
						this->_count->decStrong();
				}

				WRef<T> operator~()
				{
					return WRef<T>(this);
				}

				typename _BaseRef<T>::_Proxy operator->()
				{
					if (this->_count == nullptr)
						panic("Attempted to use moved reference");
					else
						return typename _BaseRef<T>::_Proxy(this->_count);
				}
		};

		template <typename T>
		class WRef : public _BaseRef<T>
		{
			friend class Ref<T>;

			private:
				WRef<T>& _copy(const _BaseRef<T>& other)
				{
					if (this->_count != nullptr)
						this->_count->decWeak();

					this->_count = other._count;
					this->_count->incWeak();

					return *this;
				}

				WRef<T>& _move(WRef<T>& other)
				{
					if (this->_count != nullptr)
						this->_count->decWeak();

					this->_count = other._count;
					other._count = nullptr;

					return *this;
				}

				WRef(const Ref<T>* other)
				{
					this->_copy(*other);
				}

			public:
				WRef() {}

				WRef(const WRef<T>& other)
				{
					this->_copy(other);
				}

				WRef<T>& operator=(const WRef<T>& other)
				{
					return this->_copy(other);
				}

				WRef(WRef<T>&& other)
				{
					this->_move(other);
				}

				WRef<T>& operator=(WRef<T>&& other)
				{
					return this->_move(other);
				}

				~WRef()
				{
					if (this->_count != nullptr)
						this->_count->decWeak();
				}

				bool validateAndLock()
				{
					if (this->_count == nullptr)
						return false;
					else
						return this->_count->validateAndLock();
				}

				typename _BaseRef<T>::_Proxy operator->()
				{
					if (this->_count == nullptr)
						panic("Attempted to use moved weak reference");
					else
					{
						if (!this->_count->isValid())
							panic("Attempted to use invalid weak reference");
						else
							return typename _BaseRef<T>::_Proxy(this->_count);
					}
				}
		};
	}
}

#endif
