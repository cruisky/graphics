#include "System/Thread.h"

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
			mScheduler->taskFinished.WakeOne();	// only ThreadScheduler->JoinAll waits for this condition
		}
	}

	ThreadScheduler* ThreadScheduler::instance = nullptr;
	ThreadScheduler* ThreadScheduler::Instance(){
		if (!instance){
			instance = new ThreadScheduler;
		}
		return instance;
	}
	void ThreadScheduler::DeleteInstance(){
		if (instance){
			instance->StopAll();
			delete instance;
			instance = nullptr;
		}
	}
	void ThreadScheduler::StartAll(){
		int processor_count = std::thread::hardware_concurrency();
		running = true;
		//processor_count = 2;	// TODO test single thread
		threads.resize(processor_count);
		for (int i = 0; i < processor_count; i++){
			threads[i].Init(this, i);
			threads[i].Start();
		}
	}

	void ThreadScheduler::StopAll(){
		running = false;
		taskLock.WaitAndLock();
		taskAvailable.WakeAll();
		taskLock.Unlock();
		for (int i = 0; i < threads.size(); i++){
			threads[i].Stop();
		}
	}
	void ThreadScheduler::AddTask(Task& newTask){
		taskLock.WaitAndLock();
		tasks.push_back(newTask);
		taskLock.Unlock();

		taskCount++;
		taskAvailable.WakeAll();
	}
	void ThreadScheduler::JoinAll(){
		finishedLock.WaitAndLock();
		while (taskCount > 0){
			taskFinished.Wait(finishedLock);
		}
		finishedLock.Unlock();
	}
}
