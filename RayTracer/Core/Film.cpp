#include "stdafx.h"

#include "Film.h"
#include "Color.h"

namespace Cruisky
{
	namespace RayTracer {
		void Film::Resize(int width, int height){
			width_ = width;
			height_ = height;
			pixels_.reset(new Color[width * height]);
		}

		const Color *Film::GetPixels() const {
			assert(pixels_);
			return pixels_.get();
		}

		void Film::Release(){
			width_ = height_ = 0;
			pixels_.reset();
		}
	}
}