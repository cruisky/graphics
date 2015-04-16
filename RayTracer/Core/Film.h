#pragma once
#include "fwddecl.h"

#include <memory>

namespace Cruisky{
	namespace RayTracer{
		class Film {
		public:
			Film() : width_(0), height_(0){};
			Film(int width, int height){ Resize(width, height); }
			~Film(){}

			inline int Width() const { return width_; }
			inline int Height() const { return height_; }
			
			void Commit(const Sample& sample, const Color& color);
			void Resize(int width, int height);
			void Reset();
			const Color *Pixels() const;

		private:
			int width_, height_;
			std::unique_ptr<Color[]> pixels_;	// or vector<Color> if Resize() is constantly called
		};
	}
}