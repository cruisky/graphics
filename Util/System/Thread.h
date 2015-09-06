#pragma once

#include "Util.h"
#include <Windows.h>
#undef near
#undef far

#include <thread>
#include <deque>
#include <atomic>

namespace TX{
	class ThreadScheduler;

	// General thread
	class Thread{
	public:
		Thread();
		virtual ~Thread(){
			if (mRunning){
				CloseHandle(mHandle);
				mRunning = false;
			}
		}
	public:
		inline bool Running() { return mRunning; }
		inline HANDLE Handle() { return mHandle; }
		inline DWORD TID() { return mTid; }
		void Start();
		void Stop();
		virtual void WorkLoop() = 0;
	protected:
		HANDLE mHandle;
		DWORD mTid;
		bool mRunning;
	private:
		HANDLE mStopEvent;
		static DWORD WINAPI ThreadProc(LPVOID lpParam);
	};

	// Worker thread that gets tasks from scheduler
	class WorkerThread :public Thread{
	public:
		WorkerThread(){
		}
	public:
		void Init(ThreadScheduler *scheduler, int id){
			mScheduler = scheduler;
			mId = id;
			mWaiting = true;
		}
		void WorkLoop();
	private:
		ThreadScheduler *mScheduler;
		int mId;
		bool mWaiting;
	};

	// Process local lock. Wrapper for CRITICAL_SECTION
	class Lock {
		friend class Condition;
	public:
		Lock(){
			InitializeCriticalSection(&critical_sect);
		}
		virtual ~Lock(){
			DeleteCriticalSection(&critical_sect);
		}
	public:
		void WaitAndLock(){
			EnterCriticalSection(&critical_sect);
		}
		void Unlock(){
			LeaveCriticalSection(&critical_sect);
		}
	protected:
		CRITICAL_SECTION critical_sect;
	};

	// Apply the lock during the life time
	class LockGuard{
	public:
		LockGuard(Lock& lock) :lock_(lock){
			lock_.WaitAndLock();
		}
		virtual ~LockGuard(){
			lock_.Unlock();
		}
	protected:
		Lock& lock_;
	};

	// Wrapper for CONDITION_VARIABLE
	class Condition {
	public:
		Condition(){
			InitializeConditionVariable(&condition);
		}
	public:
		void Wait(Lock& lock){
			SleepConditionVariableCS(&condition, &lock.critical_sect, INFINITE);
		}
		void WakeOne(){
			WakeConditionVariable(&condition);
		}
		void WakeAll(){
			WakeAllConditionVariable(&condition);
		}
	public:
		CONDITION_VARIABLE condition;
	};

	// Callable function
	class Task {
	public:
		typedef void(*Func)(void *args, int idx);

		Task(Func func, void *args) : func(func), args(args){}
		inline void Run(int idx){ func(args, idx); }
	private:
		Func func;
		void *args;
	};

	// ThreadScheduler that assigns tasks to threads
	class ThreadScheduler {
	public:
		// Singleton implementation
		static ThreadScheduler *Instance();
		static void DeleteInstance();
	public:
		inline bool Running(){ return running; }
		inline int ThreadCount(){ return threads.size(); }
		void StartAll();
		void StopAll();
		void AddTask(Task& newTask);
		void JoinAll();
	private:
		static ThreadScheduler *instance;
		bool running;
		ThreadScheduler() : running(false) { taskCount = 0; }

	public:
		std::atomic_int taskCount;
		std::deque<Task> tasks;
		std::vector<WorkerThread> threads;

		Lock taskLock;
		Condition taskAvailable;

		Lock finishedLock;
		Condition taskFinished;	// the completion of one task
	};
}
