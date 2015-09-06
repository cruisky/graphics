#pragma once

#include "Util.h"
#include "Color.h"

namespace TX{
	class Image {
	public:
		enum Format {
			BMP, PNG
		};

		static void Write(
			char const *filename,
			const Color *data,
			int width,
			int height,
			bool flip_y = false,
			Format format = BMP,
			Color::Channel channel = Color::Channel::RGB);
	};
}