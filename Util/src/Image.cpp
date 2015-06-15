#include "include/Image.h"
#include "include/Color.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace TX{
	void Image::Write(char const *filename, const Color *data, int width, int height, bool flip_y, Format format, Channel channel){
		// get bytes per pixel
		const int pixel_size = channel;
		const int image_size = width * height;
		
		if (width <= 0 || height <= 0)
			return;

		unsigned char *buffer = new unsigned char[image_size * pixel_size];
		int buffer_i, data_i;
		if (channel <= YA){
			for (int y = 0; y < height; y++){
				buffer_i = ((flip_y ? height - y - 1 : y) * width) * pixel_size;
				for (int x = 0, data_i = y * width; x < width; x++, data_i++){
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].Luminance() * 255), 0, 255);
					if (channel == YA)
						buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].a * 255), 0, 255);
				}
			}
		}
		else {
			for (int y = 0; y < height; y++){
				buffer_i = ((flip_y ? height - y - 1 : y) * width) * pixel_size;
				for (int x = 0, data_i = y * width; x < width; x++, data_i++){
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].r * 255), 0, 255);
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].g * 255), 0, 255);
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].b * 255), 0, 255);
					if (channel == RGBA)
						buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].a * 255), 0, 255);
				}
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