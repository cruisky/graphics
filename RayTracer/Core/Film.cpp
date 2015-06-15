#include "stdafx.h"

#include "Film.h"
#include "Sampler.h"
#include "Sample.h"
#include "Color.h"

namespace TX
{
	namespace RayTracer {
		void Film::Commit(const CameraSample& sample, const Color& color){
			int offset = sample.pix_y * width_ + sample.pix_x;
			unscaled_pixels_[offset] += color;
			weights_[offset] += 1.f;
			// TODO filter
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

		void Film::Reset(){
			memset(unscaled_pixels_.get(), 0, size_ * sizeof(Color));
			memset(weights_.get(), 0, size_ * sizeof(float));
		}

		void Film::ResetAll(){
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
}