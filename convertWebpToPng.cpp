#include "Converter.h"

std::vector<unsigned char> ImageFormatConverter::Converter::convertWebpToPng(System::String^ inputPath) {
    std::vector<unsigned char> png_data;
    std::string input_path = msclr::interop::marshal_as<std::string>(inputPath);

    std::ifstream file(input_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return png_data;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        return png_data;
    }

    WebPBitstreamFeatures features;
    VP8StatusCode status = WebPGetFeatures(reinterpret_cast<uint8_t*>(buffer.data()), size, &features);
    if (status != VP8_STATUS_OK) {
        return png_data;
    }

    int picture_width = features.width;
    int picture_height = features.height;
    bool has_alpha = features.has_alpha != 0;

    uint8_t* rgba_data;
    if (has_alpha) {
        rgba_data = WebPDecodeRGBA(reinterpret_cast<uint8_t*>(buffer.data()), size, &picture_width, &picture_height);
    }
    else {
        rgba_data = WebPDecodeRGB(reinterpret_cast<uint8_t*>(buffer.data()), size, &picture_width, &picture_height);
    }

    if (!rgba_data) {
        return png_data;
    }

    std::vector<unsigned char> image;
    if (has_alpha) {
        image = std::vector<unsigned char>(rgba_data, rgba_data + picture_width * picture_height * 4);
    }
    else {
        image.reserve(picture_width * picture_height * 4);
        for (int i = 0; i < picture_width * picture_height * 3; i += 3) {
            image.push_back(rgba_data[i]);
            image.push_back(rgba_data[i + 1]);
            image.push_back(rgba_data[i + 2]);
            image.push_back(255);
        }
    }

    WebPFree(rgba_data);

    unsigned error = lodepng::encode(png_data, image, picture_width, picture_height);
    if (error) {
        png_data.clear();
    }

    return png_data;
}