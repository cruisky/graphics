#pragma once
#include "fwddecl.h"

#include <memory>
#include "Graphics/Color.h"

namespace TX{
	namespace RayTracer{
		class Film {
		public:
			Film() : width_(0), height_(0), size_(0){};
			Film(int width, int height){ Resize(width, height); }
			~Film(){}

			inline int Width() const { return width_; }
			inline int Height() const { return height_; }
			inline int Size() const { return size_; }
			
			void Commit(const CameraSample& sample, const Color& color);
			void Resize(int width, int height);
			void Reset();						// resets weight and unscaled pixel buffer
			void ResetAll();
			Color Get(int x, int y) const;
			void ScalePixels();
			const Color *Pixels() const;

		private:
			int width_, height_, size_;
			std::unique_ptr<Color[]> pixels_;	// or vector<Color> if Resize() is constantly called
			std::unique_ptr<Color[]> unscaled_pixels_;	// or vector<Color> if Resize() is constantly called
			std::unique_ptr<float[]> weights_;
		};
	}
}