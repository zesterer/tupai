/*
* 	file : mempool.cpp
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

// Tupai
#include <tupai/mempool.hpp>
#include <tupai/kpanic.hpp>

namespace tupai
{
	typedef ubyte map_entry_data;

	static const map_entry_data MAP_ENTRY_MASK_ALL  = ~(map_entry_data)0;
	static const map_entry_data MAP_ENTRY_MASK_NONE = (map_entry_data)0;

	static const map_entry_data MAP_ENTRY_ALL  = ~(map_entry_data)0;
	static const map_entry_data MAP_ENTRY_NONE = (map_entry_data)0;

	struct map_entry
	{
		enum class flag_type
		{
			CLEAR = 0,
			USED  = (1 << 0),
			HEAD  = (1 << 1),
		};

		map_entry_data flags;

		void clear() { this->flags = (map_entry_data)flag_type::CLEAR; }

		bool get_flag(flag_type flag) { return (this->flags & (map_entry_data)flag) > 0; }
		void set_flag(flag_type flag, bool enabled) { if (enabled) this->flags |= (map_entry_data)flag; else this->flags &= ~(map_entry_data)flag; }

		bool get_used() { return this->get_flag(flag_type::USED); }
		bool get_head() { return this->get_flag(flag_type::HEAD); }

		void set_used(bool used) { this->set_flag(flag_type::USED, used); }
		void set_head(bool head) { this->set_flag(flag_type::HEAD, head); }

		bool matches(map_entry_data match, map_entry_data mask = MAP_ENTRY_MASK_ALL) { return (this->flags & mask) == match; }
		void flag(map_entry_data flags, map_entry_data mask = MAP_ENTRY_MASK_ALL) { this->flags = (flags & mask) | (this->flags & ~mask); }
	};

	static const map_entry_data MAP_ENTRY_MASK_USED = (map_entry_data)map_entry::flag_type::USED;
	static const map_entry_data MAP_ENTRY_MASK_HEAD = (map_entry_data)map_entry::flag_type::HEAD;

	static const map_entry_data MAP_ENTRY_USED = (map_entry_data)map_entry::flag_type::USED;
	static const map_entry_data MAP_ENTRY_HEAD = (map_entry_data)map_entry::flag_type::HEAD;

	static const umem MAP_INDEX_INVALID = ~(umem)0;

	struct map_region
	{
		umem start;
		umem size;

		map_region(umem start, umem size) { this->start = start; this->size = size; }
		bool valid() { return this->start != MAP_INDEX_INVALID && this->size != MAP_INDEX_INVALID; }
	};

	static void* mempool_space_head = nullptr;
	static umem  mempool_space_size = 0;

	static void* mempool_map_head = nullptr;
	static umem  mempool_map_size = 0;

	static void* mempool_pool_head = nullptr;
	static umem  mempool_pool_size = 0;

	static map_entry* mempool_map = nullptr;
	static umem       mempool_block_size = 0;
	static umem       mempool_block_count = 0;
	static bool       mempool_initiated = false;

	static void* mempool_index_to_ptr(umem index);
	static umem  mempool_ptr_to_index(void* ptr);
	static umem  mempool_size_to_mapcount(umem size);

	static map_region mempool_find_region(umem size, map_entry_data match, map_entry_data mask = MAP_ENTRY_MASK_ALL, umem offset = 0);
	static bool       mempool_flag_region(map_region region, map_entry_data flags, map_entry_data mask = MAP_ENTRY_MASK_ALL);

	static map_region mempool_find_chain(umem index);

	static bool mempool_transcribe_block(umem src, umem dest);
	static bool mempool_transcribe_region(map_region src, map_region dest);

	static void  mempool_init_impl(void* ptr, umem size, umem blocksize);
	static void* mempool_alloc_impl(umem n);
	static void* mempool_realloc_impl(void* ptr, umem n);
	static void  mempool_dealloc_impl(void* ptr);

	static void* mempool_index_to_ptr(umem index)
	{
		if (index > mempool_block_count) // Is the index out of bounds?
			return nullptr;

		return (void*)((umem)mempool_pool_head + index * mempool_block_size);
	}

	static umem mempool_ptr_to_index(void* ptr)
	{
		if (((umem)ptr - (umem)mempool_pool_head) % mempool_block_size != 0) // Is the pointer not aligned correctly?
			return MAP_INDEX_INVALID;

		if (ptr < mempool_pool_head || (umem)ptr >= (umem)mempool_pool_head + mempool_pool_size) // Is the pointer out of bounds?
			return MAP_INDEX_INVALID;

		return ((umem)ptr - (umem)mempool_pool_head) / mempool_block_size;
	}

	static umem mempool_size_to_mapcount(umem size)
	{
		if (size % mempool_block_size != 0) // If it's perfectly block-aligned
			return size / mempool_block_size + 1;
		else // If it's not perfectly block-aligned
			return size / mempool_block_size;
	}

	static map_region mempool_find_region(umem size, map_entry_data match, map_entry_data mask, umem offset)
	{
		umem cpos = offset;
		umem csize = 0;

		for (umem i = offset; i < mempool_block_count; i ++)
		{
			if (mempool_map[i].matches(match, mask)) // If we have a matching entry
				csize ++;
			else // If the entry does not match
			{
				cpos = i + 1;
				csize = 0;
			}

			if (csize >= size) // If we've found a large enough region
				return map_region(cpos, csize);
		}

		return map_region(MAP_INDEX_INVALID, MAP_INDEX_INVALID);
	}

	static bool mempool_flag_region(map_region region, map_entry_data flags, map_entry_data mask)
	{
		if (!region.valid())
			return false;

		for (umem i = 0; i < region.size; i ++)
		{
			if (region.start + i >= mempool_block_count) // If we're outside the map bounds
				return false;
			else
				mempool_map[region.start + i].flag(flags, mask); // Flag each entry
		}

		return true;
	}

	static map_region mempool_find_chain(umem index)
	{
		umem cpos = index;
		umem csize = 0;

		if (mempool_map[cpos].matches(MAP_ENTRY_USED | MAP_ENTRY_HEAD, MAP_ENTRY_MASK_USED | MAP_ENTRY_MASK_HEAD)) // If we've found a head entry
			csize ++;
		else // If we've haven't found a head entry
			return map_region(MAP_INDEX_INVALID, MAP_INDEX_INVALID);

		for (umem i = cpos + 1; i < mempool_block_count; i ++)
		{
			if (mempool_map[i].matches(MAP_ENTRY_USED, MAP_ENTRY_MASK_USED | MAP_ENTRY_MASK_HEAD)) // If we've found a tail entry
				csize ++;
			else // If we've not found a tail entry
				break;
		}

		return map_region(cpos, csize);
	}

	static bool mempool_transcribe_block(umem src, umem dest)
	{
		if (src == dest) // If we're transcribing the same block
			return false;

		void* src_ptr = mempool_index_to_ptr(src);
		void* dest_ptr = mempool_index_to_ptr(dest);

		if (src_ptr == nullptr || dest_ptr == nullptr) // If either map index was invalid
			return false;

		for (umem i = 0; i < mempool_block_size; i ++)
		{
			((ubyte*)dest_ptr)[i] = ((ubyte*)src_ptr)[i];
		}

		return true;
	}

	static bool mempool_transcribe_region(map_region src, map_region dest)
	{
		if (!src.valid() || !dest.valid()) // If either src or dest are invalid
			return false;

		if ((src.start <= dest.start && dest.start < src.start + src.size) || (dest.start <= src.start && src.start < dest.start + dest.size)) // If an overlap occurs
			return false;

		for (umem i = 0; (i < src.size) && (i < dest.size); i ++)
		{
			if (!mempool_transcribe_block(src.start + i, dest.start + i)) // If the transcribe failed
				return false;
		}

		return true;
	}

	static void mempool_init_impl(void* ptr, umem size, umem blocksize)
	{
		if (mempool_initiated)
			return kpanic("Mempool init failure: re-initialized");

		mempool_space_head = ptr;
		mempool_space_size = size;

		mempool_block_size = blocksize;
		mempool_block_count = mempool_space_size / (sizeof(map_entry) + mempool_block_size);

		mempool_map_head = mempool_space_head;
		mempool_map_size = mempool_block_count * sizeof(map_entry);

		mempool_pool_head = (void*)((umem)mempool_map_head + mempool_map_size);
		mempool_pool_size = mempool_block_count * mempool_block_size;

		mempool_map = (map_entry*)mempool_map_head;

		mempool_initiated = true;
	}

	static void* mempool_alloc_impl(umem n)
	{
		umem block_num = mempool_size_to_mapcount(n); // How many blocks will we need?

		map_region new_region = mempool_find_region(block_num, 0, MAP_ENTRY_MASK_USED); // Find an appropriate region with no USED flags

		if (new_region.valid())
		{
			mempool_flag_region(map_region(new_region.start, 1), MAP_ENTRY_USED | MAP_ENTRY_HEAD, MAP_ENTRY_MASK_USED | MAP_ENTRY_MASK_HEAD); // Flag the new head

			if (new_region.size > 1)
				mempool_flag_region(map_region(new_region.start + 1, new_region.size - 1), MAP_ENTRY_USED, MAP_ENTRY_MASK_USED | MAP_ENTRY_MASK_HEAD); // Flag the new tails

			return mempool_index_to_ptr(new_region.start);
		}
		else
			kpanic("Mempool allocation failure: invalid region");
	}

	static void* mempool_realloc_impl(void* ptr, umem n)
	{
		void* dest_ptr = mempool_alloc(n);

		if (dest_ptr == nullptr)
			kpanic("Mempool reallocation failure: unsuccessful allocation");

		umem src_index = mempool_ptr_to_index(ptr);
		umem dest_index = mempool_ptr_to_index(dest_ptr);

		if (src_index == MAP_INDEX_INVALID || dest_index == MAP_INDEX_INVALID)
			kpanic("Mempool reallocation failure: source or destination index invalid");

		umem block_num = mempool_size_to_mapcount(n);

		if (!mempool_transcribe_region(map_region(src_index, block_num), map_region(dest_index, block_num)))
			kpanic("Mempool reallocation failure: Unsuccessful region transcribe");

		mempool_dealloc(ptr);

		return dest_ptr;
	}

	static void mempool_dealloc_impl(void* ptr)
	{
		umem map_index = mempool_ptr_to_index(ptr);

		if (map_index == MAP_INDEX_INVALID)
			return kpanic("Mempool deallocation failure: Invalid index");

		map_region mem_region = mempool_find_chain(map_index);

		if (mem_region.valid())
		{
			mempool_flag_region(mem_region, MAP_ENTRY_NONE, MAP_ENTRY_ALL); // Flag everything to nothing
		}
		else
			return kpanic("Mempool deallocation failure: Invalid region");
	}

	void mempool_init(void* ptr, umem size, umem blocksize)
	{
		mempool_init_impl(ptr, size, blocksize);
	}

	void* mempool_alloc(umem n)
	{
		void* result = mempool_alloc_impl(n);
		return result;
	}

	void* mempool_realloc(void* ptr, umem n)
	{
		void* result = mempool_realloc_impl(ptr, n);
		return result;
	}

	void mempool_dealloc(void* ptr)
	{
		mempool_dealloc_impl(ptr);
	}
}
