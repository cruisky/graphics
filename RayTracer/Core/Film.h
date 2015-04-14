#pragma once
#include "fwddecl.h"

#include <memory>

namespace Cruisky{
	namespace RayTracer{
		class Film {
		public:
			Film(int width, int height){ Resize(width, height); }
			~Film(){ Release(); }

			//TODO
			//void Commit(Sample& sample, Color& color);

			inline int Width() const { return width_; }
			inline int Height() const { return height_; }
			void Resize(int width, int height);
			const Color *GetPixels() const;
			
			void Release();

		private:
			int width_, height_;
			std::unique_ptr<Color[]> pixels_;	// or vector<Color> if Resize() is constantly called
		};
	}
}