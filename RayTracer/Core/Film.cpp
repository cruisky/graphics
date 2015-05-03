#include "stdafx.h"

#include "Film.h"
#include "Sampler.h"
#include "Sample.h"
#include "Color.h"

namespace Cruisky
{
	namespace RayTracer {
		void Film::Commit(const CameraSample& sample, const Color& color){
			int offset = sample.pix_y * width_ + sample.pix_x;
			pixels_[offset] = color;
			// TODO filter
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

		Color Film::Get(int x, int y) const{
			return pixels_[y * width_ + x];
		}

		const Color *Film::Pixels() const {
			assert(pixels_);
			return pixels_.get();
		}
	}
}