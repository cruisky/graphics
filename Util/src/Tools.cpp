#include "include/Tools.h"

namespace Cruisky
{
	void ProgressMonitor::Reset(float total){
		timer_.reset();
		in_progress_ = true;
		total_ = total;
		current_ = 0.f;
		time_since_last_update_ = 0.0;
	}
	void ProgressMonitor::Update(float current){
		assert(in_progress_);
		double elapsed = timer_.elapsed();
		rate_per_sec_ = (current - current_) / (double)(elapsed - time_since_last_update_);
		current_ = current;
		time_since_last_update_ = elapsed;
	}
	void ProgressMonitor::Finish(){
		Update(total_);
		in_progress_ = false;
	}
	double ProgressMonitor::ElapsedTime(){
		return in_progress_ ? timer_.elapsed() : time_since_last_update_;
	}
	double ProgressMonitor::RemainingTime(){
		float est = EstimatedProgress();
		return in_progress_ ? (timer_.elapsed() * (1.f - est) / est) : 0.0;
	}
	float ProgressMonitor::EstimatedProgress(){
		return in_progress_ ? ((current_ + rate_per_sec_ * (float)(timer_.elapsed() - time_since_last_update_)) / total_) : 1.f;
	}
}