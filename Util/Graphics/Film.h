#pragma once
#include "Util.h"
#include <memory>
#include "Graphics/Color.h"

namespace TX{
	enum class FilterType{
		BoxFilter,
		GaussianFilter
	};

	class Film {
	public:
		Film(FilterType filter_t = FilterType::BoxFilter) : width_(0), height_(0), size_(0){ SetFilter(filter_t); };
		Film(int width, int height, FilterType filter_t) {
			Resize(width, height);
			SetFilter(filter_t);
		}
		~Film(){}

		inline int Width() const { return width_; }
		inline int Height() const { return height_; }
		inline int Size() const { return size_; }

		void Commit(float x, float y, const Color& color);
		void Resize(int width, int height);
		void SetFilter(FilterType filter_t);

		/// <summary>
		/// Rresets weight and unscaled pixel buffer, and leave the scaled pixel buffer untouched.
		/// </summary>
		void Reset();
		void Clear();
		Color Get(int x, int y) const;
		void ScalePixels();
		const Color *Pixels() const;

	private:
		int width_, height_, size_;
		std::unique_ptr<Color[]> pixels_;	// or vector<Color> if Resize() is constantly called
		std::unique_ptr<Color[]> unscaled_pixels_;	// or vector<Color> if Resize() is constantly called
		std::shared_ptr<Filter> filter_;
		std::unique_ptr<float[]> weights_;
	};
}
