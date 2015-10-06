#pragma once

#include "Util.h"
#include "Thread.h"

namespace TX {
	std::string ReadAllLines(const std::string& file);

	class Timer
	{
	public:
		Timer() : beg_(clock_::now()) {}
		__forceinline void reset() { beg_ = clock_::now(); }
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
		virtual void UpdateInc() = 0;
		virtual void Finish() = 0;
		virtual double ElapsedTime() = 0;
		virtual double RemainingTime() = 0;
		virtual float Progress() = 0;
		virtual bool InProgress() = 0;
	};

	class ProgressMonitor : public IProgressMonitor {
	public:
		ProgressMonitor(){}
		// Reset the total value, and start timer immediately
		void Reset(float total);
		void Finish();
		void Update(float current);
		void UpdateInc();
		double ElapsedTime();
		double RemainingTime();
		inline float Progress() { return current_ / total_; }
		inline bool InProgress(){ return in_progress_; }
	private:
		float EstimatedProgress();
	private:
		float total_, current_, rate_per_sec_;
		bool in_progress_;
		std::atomic<double> time_since_last_update_;
		Timer timer_;
		Lock lock_;
	};

	template<typename T>
	void Str_(std::ostringstream& ss, const T& obj){
		ss << obj;
	}

	template<typename T, typename... Args>
	void Str_(std::ostringstream& ss, const T& obj, Args&&... args){
		Str_(ss, obj);
		Str_(ss, std::forward<Args>(args)...);
	}

	template<typename... Args>
	std::string Str(Args&&... args){
		std::ostringstream ss;
		Str_(ss, std::forward<Args>(args)...);
		return ss.str();
	}
}
