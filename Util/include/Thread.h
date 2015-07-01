#pragma once

#include "../Util.h"
#include <Windows.h>
#include <thread>
#include <deque>
#include <atomic>

namespace TX{
	class Scheduler;

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
		void Init(Scheduler *scheduler, int id){
			mScheduler = scheduler;
			mId = id;
			mWaiting = true;
		}
		void WorkLoop();
	private:
		Scheduler *mScheduler;
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
		inline void Run(int idx){
			func(args, idx);
		}
	private:
		Func func;
		void *args;
	};

	// Scheduler that assigns tasks to threads
	class Scheduler {
	public:
		// Singleton implementation
		static Scheduler *Instance(){
			if (!instance){
				instance = new Scheduler;
			}
			return instance;
		}
		static void DeleteInstance(){
			if (instance){
				instance->Stop();
				delete instance;
				instance = nullptr;
			}
		}
	public:
		void Start();
		void Stop();
		inline bool Running(){ return running; }
		int ThreadCount(){
			return threads.size();
		}
		void AddTask(Task& newTask){
			taskLock.WaitAndLock();
			tasks.push_back(newTask);
			taskLock.Unlock();

			taskCount++;
			taskAvailable.WakeAll();
		}
		void JoinAll(){
			finishedLock.WaitAndLock();
			while (taskCount > 0){
				taskFinished.Wait(finishedLock);
			}
			finishedLock.Unlock();
		}
	private:
		static Scheduler *instance;
		bool running;
		Scheduler() : running(false) {
			taskCount = 0;
		}

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