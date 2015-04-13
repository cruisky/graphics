#include "include/Image.h"
#include "include/Color.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Cruisky{
	void Image::Write(char const *filename, const Color *data, int width, int height, Format format, Channel channel){
		// get bytes per pixel
		const int pixel_size = channel;
		const int image_size = width * height;
		unsigned char *buffer = new unsigned char[image_size * pixel_size];
		if (channel <= YA){
			for (int i = 0; i < image_size; i++){
				int buffer_i = i * pixel_size;
				buffer[buffer_i] = Math::Clamp(Math::Round(data[i].Luminance() * 255), 0, 255);
				if (channel == YA)
					buffer[buffer_i + 1] = Math::Clamp(Math::Round(data[i].a * 255), 0, 255);
			}
		}
		else {
			for (int i = 0; i < image_size; i++){
				int buffer_i = i * pixel_size;
				buffer[buffer_i + 0] = Math::Clamp(Math::Round(data[i].r * 255), 0, 255);
				buffer[buffer_i + 1] = Math::Clamp(Math::Round(data[i].g * 255), 0, 255);
				buffer[buffer_i + 2] = Math::Clamp(Math::Round(data[i].b * 255), 0, 255);
				if (channel == RGBA)
					buffer[buffer_i + 3] = Math::Clamp(Math::Round(data[i].a * 255), 0, 255);
			}
		}

		switch (format){
		case PNG:
			stbi_write_png(filename, width, height, channel, buffer, width * pixel_size);
			break;
		case BMP:
			stbi_write_bmp(filename, width, height, channel, buffer);
			break;
		}
		delete[] buffer;
	}
}