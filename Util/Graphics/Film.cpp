#include "UtilStdAfx.h"
#include "Film.h"
#include "Math/Sampler.h"
#include "Math/Sample.h"
#include "Graphics/Filter.h"

namespace TX
{
	void Film::Commit(float x, float y, const Color& color){
		using namespace Math;
#ifndef _DEBUG
		if (!color.Valid()) return;
#endif
		int offset;
		x -= 0.5f;
		y -= 0.5f;
		int xmin = Max(0, Ceil(x - filter_->radius));
		int xmax = Min(Floor(x + filter_->radius), width_ - 1);
		int ymin = Max(0, Ceil(y - filter_->radius));
		int ymax = Min(Floor(y + filter_->radius), height_ - 1);
		for (int j = ymin; j <= ymax; j++){
			for (int i = xmin; i <= xmax; i++){
				float weight = filter_->Eval(i - x, j - y);
				offset = j * width_ + i;
				unscaled_pixels_[offset] += color * weight;
				weights_[offset] += weight;
			}
		}
	}

	void Film::Resize(int width, int height){
		if (width_ != width || height_ != height){
			width_ = width;
			height_ = height;
			size_ = width_ * height_;
			pixels_.reset(new Color[width * height]);
			unscaled_pixels_.reset(new Color[width * height]);
			weights_.reset(new float[width * height]);
		}
	}

	void Film::SetFilter(FilterType filter_t) {
		switch (filter_t){
		case FilterType::BoxFilter:
			filter_.reset(new BoxFilter);
			break;
		case FilterType::GaussianFilter:
			filter_.reset(new GaussianFilter);
			break;
		default:
			throw "unimplemented";
		}
	}

	void Film::Reset(){
		memset(unscaled_pixels_.get(), 0, size_ * sizeof(Color));
		memset(weights_.get(), 0, size_ * sizeof(float));
	}

	void Film::Clear(){
		memset(pixels_.get(), 0, size_ * sizeof(Color));
		Reset();
	}

	Color Film::Get(int x, int y) const{
		return pixels_[y * width_ + x];
	}

	void Film::ScalePixels(){
		for (int i = 0; i < size_; i++)
			pixels_[i] = unscaled_pixels_[i] / weights_[i];
	}

	const Color *Film::Pixels() const {
		return pixels_.get();
	}
}
