#pragma once

#include "fwddecl.h"

namespace Cruisky{
	class Image {
	public:
		enum Channel {
			Y = 1, YA, RGB, RGBA
		};

		enum Format {
			BMP, PNG
		};

		static void Write(
			char const *filename,
			const Color *data,
			int width,
			int height,
			Format format = BMP,
			Channel channel = RGB);
	};
}