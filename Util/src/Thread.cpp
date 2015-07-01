#include "include/Thread.h"

namespace TX
{
	Thread::Thread() : mHandle(NULL), mTid(0), mRunning(false)
	{
		mStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	void Thread::Start(){
		if (mRunning)
			return;
		DWORD new_tid;
		mHandle = CreateThread(
			NULL,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(&ThreadProc),
			this,
			0,
			&new_tid);
		if (mHandle){
			mTid = new_tid;
			mRunning = true;
		}
	}
	void Thread::Stop(){
		if (mRunning){
			SetEvent(mStopEvent);
			WaitForSingleObject(mHandle, INFINITE);
			CloseHandle(mHandle);
			mRunning = false;
		}
	}
	DWORD WINAPI Thread::ThreadProc(LPVOID lpParam){
		Thread* thread = (Thread *)lpParam;
		while (WaitForSingleObject(thread->mStopEvent, 0) == WAIT_TIMEOUT){
			thread->WorkLoop();
		}
		return 0;
	}
	
	// Get a task from the scheduler and run it
	void WorkerThread::WorkLoop(){
		mScheduler->taskLock.WaitAndLock();
		while (mScheduler->Running() && mScheduler->tasks.empty()){
			mScheduler->taskAvailable.Wait(mScheduler->taskLock);
		}
		if (!mScheduler->Running()){
			mScheduler->taskLock.Unlock();
			return;
		}

		Task& task = mScheduler->tasks.front();
		mScheduler->tasks.pop_front();
		mScheduler->taskLock.Unlock();

		task.Run(mId);

		if (--mScheduler->taskCount == 0){
			mScheduler->taskFinished.WakeOne();	// only Scheduler->JoinAll waits for this condition
		}
	}

	void Scheduler::Start(){
		int availableCores = std::thread::hardware_concurrency();
		running = true;
		threads.resize(availableCores);
		for (int i = 0; i < availableCores; i++){
			threads[i].Init(this, i);
			threads[i].Start();
		}
	}

	void Scheduler::Stop(){
		running = false;
		taskLock.WaitAndLock();
		taskAvailable.WakeAll();
		taskLock.Unlock();
		for (int i = 0; i < threads.size(); i++){
			threads[i].Stop();
		}
	}
}