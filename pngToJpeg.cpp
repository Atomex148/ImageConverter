#include "Converter.h"

int ImageFormatConverter::Converter::pngToJpeg(System::String^ inputPath) {
	tjhandle compressor = tjInitCompress();
	if (compressor == NULL) return -1;

	std::string path_to_png = msclr::interop::marshal_as<std::string>(inputPath);
	if (path_to_png == "") return -1;

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, path_to_png);
	if (error) return -1;

	std::vector<unsigned char> imageRGB(width * height * 3);
	for (unsigned i = 0, j = 0; i < width * height * 4; i += 4, j += 3) {
		if (image[i + 3] == 0) {
			imageRGB[j] = 255;
			imageRGB[j + 1] = 255;
			imageRGB[j + 2] = 255;
		}
		else {
			imageRGB[j] = image[i];
			imageRGB[j + 1] = image[i + 1];
			imageRGB[j + 2] = image[i + 2];
		}
	}
	
	unsigned char* jpegBuf = NULL;
	unsigned long jpegSize = 0;

	int result = tjCompress2(compressor, imageRGB.data(), width, 0, height, TJPF_RGB,
		&jpegBuf, &jpegSize, TJSAMP_444, 75, TJFLAG_FASTDCT);
	if (result != 0) {
		tjFree(jpegBuf);
		tjDestroy(compressor);
		return -1;
	}

	std::size_t lastDot = path_to_png.find_last_of(".");
	std::string outputPath = path_to_png.substr(0, lastDot) + ".jpg";

	FILE* outputFile = fopen(outputPath.c_str(), "wb");
	if (!outputFile) {
		tjFree(jpegBuf);
		tjDestroy(compressor);
		return -1;
	}

	fwrite(jpegBuf, jpegSize, 1, outputFile);
	fclose(outputFile);
	tjFree(jpegBuf);
	tjDestroy(compressor);

	return 0;

}