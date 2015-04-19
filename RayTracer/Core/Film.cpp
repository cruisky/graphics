#include "stdafx.h"

#include "Film.h"
#include "Sampler.h"
#include "Color.h"

namespace Cruisky
{
	namespace RayTracer {
		void Film::Commit(const Sample& sample, const Color& color){
			int offset = int(sample.v) * width_ + int(sample.u);
			pixels_[offset] = color;
		}

		void Film::Resize(int width, int height){
			if (width_ != width || height_ != height){
				width_ = width;
				height_ = height;
				pixels_.reset(new Color[width * height]);
			}
		}

		void Film::Reset(){
			int size = width_ * height_;
			for (int i = 0; i < size; i++)
				pixels_[i] = Color::BLACK;
		}

		const Color *Film::Pixels() const {
			assert(pixels_);
			return pixels_.get();
		}
	}
}