#pragma once

namespace TX
{
	class NonCopyable{
	protected:
		NonCopyable(){}
	private:
		NonCopyable(NonCopyable const &);
		NonCopyable& operator = (NonCopyable const &);
	};
	template<class T>
	class AutoDelete : NonCopyable{
	public:
		AutoDelete(T *p = 0) : ptr_(p) {}
		~AutoDelete() throw() { delete ptr_; }
	private:
		T *ptr_;
	};
	template<class T>
	class AutoDeleteArray : NonCopyable{
	public:
		AutoDeleteArray(T *p = 0) : ptr_(p) {}
		~AutoDeleteArray() throw() { delete[] ptr_; }
	private:
		T *ptr_;
	};

	template<class T>
	inline void MemDelete(T*& ptr){
		if (ptr){
			delete ptr;
			ptr = nullptr;
		}
	}
	template<class T>
	inline void MemDeleteArray(T*& ptr){
		if (ptr){
			delete[] ptr;
			ptr = nullptr;
		}
	}
	template<class T>
	inline void MemClear(T* ptr, size_t count){
		if (ptr)
			memset(ptr, 0, sizeof(T) * count);
	}
}