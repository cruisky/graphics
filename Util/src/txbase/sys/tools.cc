#include "txbase/stdafx.h"
#include "txbase/sys/tools.h"

namespace TX
{
	std::string ReadAllLines(const std::string& file){
		std::stringstream buf;
		buf << std::ifstream(file).rdbuf();
		return buf.str();
	}

	void ProgressMonitor::Reset(float total){
		timer_.reset();
		in_progress_ = true;
		total_ = total;
		current_ = 0.f;
		time_since_last_update_ = 0.0;
	}
	void ProgressMonitor::Update(float current){
		assert(in_progress_);
		LockGuard locking(lock_);
		double elapsed = timer_.elapsed();
		float time_advanced = (float)(elapsed - time_since_last_update_);
		if (time_advanced > 0)
			rate_per_sec_ = (current - current_) / time_advanced;
		current_ = current;
		time_since_last_update_ = elapsed;
	}
	void ProgressMonitor::UpdateInc(){
		LockGuard locking(lock_);
		double elapsed = timer_.elapsed();
		float time_advanced = (float)(elapsed - time_since_last_update_);
		if (time_advanced > 0)
			rate_per_sec_ = 1.f / time_advanced;
		current_ += 1.f;
		time_since_last_update_ = elapsed;
	}
	void ProgressMonitor::Finish(){
		Update(total_);
		in_progress_ = false;
		time_since_last_update_ = timer_.elapsed();
	}
	double ProgressMonitor::ElapsedTime(){
		LockGuard locking(lock_);
		return in_progress_ ? timer_.elapsed() : time_since_last_update_.load();
	}
	double ProgressMonitor::RemainingTime(){
		float est = EstimatedProgress();
		return in_progress_ ? (timer_.elapsed() * (1.f - est) / est) : 0.0;
	}
	float ProgressMonitor::EstimatedProgress(){
		LockGuard locking(lock_);
		return in_progress_ ? ((current_ + rate_per_sec_ * (float)(timer_.elapsed() - time_since_last_update_)) / total_) : 1.f;
	}
}
