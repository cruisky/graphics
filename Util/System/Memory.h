#pragma once

namespace TX
{
	class NonCopyable{
	protected:
		NonCopyable(){}
		NonCopyable& operator = (NonCopyable const &) = delete;
	private:
		NonCopyable(NonCopyable const &);
	};
	template<typename T>
	class AutoDelete : NonCopyable{
	public:
		AutoDelete(T *p = 0) : ptr_(p) {}
		~AutoDelete() throw() { delete ptr_; }
	private:
		T *ptr_;
	};
	template<typename T>
	class AutoDeleteArray : NonCopyable{
	public:
		AutoDeleteArray(T *p = 0) : ptr_(p) {}
		~AutoDeleteArray() throw() { delete[] ptr_; }
	private:
		T *ptr_;
	};

	template<typename T>
	inline void MemDelete(T*& ptr){
		if (ptr){
			delete ptr;
			ptr = nullptr;
		}
	}
	template<typename T>
	inline void MemDeleteArray(T*& ptr){
		if (ptr){
			delete[] ptr;
			ptr = nullptr;
		}
	}
	template<typename T>
	inline void MemClear(T* ptr, size_t count){
		if (ptr)
			memset(ptr, 0, sizeof(T) * count);
	}

	template <typename T>
	inline T* AllocAligned(uint count, size_t alignment = 64) {
		return (T *)_aligned_malloc(count * sizeof(T), alignment);
	}

	template <typename T>
	inline void FreeAligned(T*& ptr) {
		if (ptr) {
			_aligned_free(ptr);
			ptr = nullptr;
		}
	}


	class MemoryArena {
	private:
		uint currBlockPos, blockSize;
		char *currBlock;
		std::vector<char *> used, available;
	public:
		MemoryArena(uint blockSize = 32768) :
			blockSize(blockSize),
			currBlockPos(0){
			currBlock = AllocAligned<char>(blockSize);
		}
		~MemoryArena() {
			FreeAligned(currBlock);
			for (size_t i = 0; i < used.size(); i++)
				FreeAligned(used[i]);
			for (size_t i = 0; i < available.size(); i++)
				FreeAligned(available[i]);
		}

		template <typename T>
		inline T* Alloc(uint count = 1) {
			size_t size = sizeof(T) * count;
			size = ((size + 15) & (~15));							// round up to 16 bytes alignment
			if (currBlockPos + size > blockSize) {					// if current block doesn't have enough room
				used.push_back(currBlock);					//	 mark current block as used
				if (available.size() && size <= blockSize) {	// take one from available (if there is any block)
					currBlock = available.back();
					available.pop_back();
				}
				else {												// allocate a new block
					currBlock = AllocAligned<char>(Math::Max(size, blockSize));
				}
				currBlockPos = 0;
			}
			T *ret = (T *)(currBlock + currBlockPos);
			currBlockPos += size;									// update the pointer to next allocation
			return ret;
		}
		void FreeAll() {
			// mark all blocks as available
			currBlockPos = 0;
			while (used.size()) {
				available.push_back(used.back());
				used.pop_back();
			}
		}
	};
}
