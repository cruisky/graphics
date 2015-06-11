#pragma once

#include "../Util.h"
#include <chrono>

namespace Cruisky{
	class Timer
	{
	public:
		Timer() : beg_(clock_::now()) {}
		void reset() { beg_ = clock_::now(); }
		__forceinline double elapsed() const {
			return std::chrono::duration_cast<second_>
				(clock_::now() - beg_).count();
		}

	private:
		typedef std::chrono::high_resolution_clock clock_;
		typedef std::chrono::duration<double, std::ratio<1> > second_;
		std::chrono::time_point<clock_> beg_;
	};

	class IProgressMonitor{
	public:
		virtual ~IProgressMonitor(){}
		virtual void Reset(float total) = 0;
		virtual void Update(float current) = 0;
		virtual void Finish() = 0;
		virtual double ElapsedTime() = 0;
		virtual double RemainingTime() = 0;
		virtual float Progress() = 0;
		virtual bool InProgress() = 0;
	};

	class ProgressMonitor : public IProgressMonitor {
	public:
		ProgressMonitor(){}
		void Reset(float total);
		void Finish();
		void Update(float current);
		double ElapsedTime();
		double RemainingTime();
		inline float Progress() { return current_ / total_; }
		inline bool InProgress(){ return in_progress_; }
	private:
		float EstimatedProgress();
	private:
		float total_, current_, rate_per_sec_;
		bool in_progress_;
		double time_since_last_update_;
		Timer timer_;
	};
}