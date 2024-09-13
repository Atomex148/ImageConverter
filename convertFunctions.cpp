#include "Converter.h"

inline std::vector<unsigned char> ImageFormatConverter::Converter::webpToPng_forProgramImage(System::String^ inputPath) {
    std::vector<unsigned char> png_data;
    std::string input_path = msclr::interop::marshal_as<std::string>(inputPath);

    std::ifstream file(input_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        file.close();
        return png_data;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        file.close();
        return png_data;
    }
    file.close();

    WebPBitstreamFeatures features;
    VP8StatusCode status = WebPGetFeatures(reinterpret_cast<uint8_t*>(buffer.data()), size, &features);
    if (status != VP8_STATUS_OK) return png_data;

    int picture_width = features.width;
    int picture_height = features.height;
    bool has_alpha = features.has_alpha != 0;

    uint8_t* rgba_data;
    if (has_alpha) rgba_data = WebPDecodeRGBA(reinterpret_cast<uint8_t*>(buffer.data()), size, &picture_width, &picture_height);
    else rgba_data = WebPDecodeRGB(reinterpret_cast<uint8_t*>(buffer.data()), size, &picture_width, &picture_height);

    if (!rgba_data) return png_data;

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
    if (error) png_data.clear();

    return png_data;
}

inline int ImageFormatConverter::Converter::webpToPng(System::String^ inputPath) {
    System::String^ savePath = System::IO::Path::ChangeExtension(inputPath, L".png");
    this->img_to_convert->Image->Save(savePath);
    return 0;
}

inline int ImageFormatConverter::Converter::pngToJpeg(System::String^ inputPath) {
    std::string path_to_png = msclr::interop::marshal_as<std::string>(inputPath);
    if (path_to_png == "") return -1;

    tjhandle compressor = tjInitCompress();
    if (compressor == NULL) return -1;

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
        &jpegBuf, &jpegSize, TJSAMP_444, 100, TJFLAG_FASTDCT);
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

inline int ImageFormatConverter::Converter::jpegToPng(System::String^ inputPath) {
    std::string path_to_png = msclr::interop::marshal_as<std::string>(inputPath);
    if (path_to_png == "") return -1;

    tjhandle decompressor = tjInitDecompress();
    if (decompressor == NULL) return -1;

    std::ifstream file(path_to_png, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        tjDestroy(decompressor);
        return -1;
    }

    std::streamsize size = file.tellg();
    std::vector<unsigned char> buffer(size);
    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        tjDestroy(decompressor);
        file.close();
        return -1;
    }
    file.close();

    int width, height, subsamp, colorspace;
    if (tjDecompressHeader3(decompressor, buffer.data(), size, &width, &height, &subsamp, &colorspace) < 0) {
        tjDestroy(decompressor);
        return -1;
    }

    std::vector<unsigned char> image(width * height * 3);
    if (tjDecompress2(decompressor, buffer.data(), size, image.data(), width, 0, height, TJPF_RGB, 0) < 0) {
        tjDestroy(decompressor);
        return -1;
    }

    std::vector<unsigned char> imagePNG(width * height * 4);
    for (int i = 0, j = 0; i < image.size(); i += 3, j += 4) {
        imagePNG[j] = image[i];
        imagePNG[j + 1] = image[i + 1];
        imagePNG[j + 2] = image[i + 2];
        imagePNG[j + 3] = 255;
    }

    std::vector<unsigned char> png;
    unsigned error = lodepng::encode(png, imagePNG, width, height);
    if (error) {
        tjDestroy(decompressor);
        return -1;
    }

    std::size_t lastDot = path_to_png.find_last_of(".");
    std::string outputPath = path_to_png.substr(0, lastDot) + ".png";

    FILE* outputFile = fopen(outputPath.c_str(), "wb");
    if (!outputFile) {
        tjDestroy(decompressor);
        return -1;
    }

    fwrite(png.data(), png.size(), 1, outputFile);
    fclose(outputFile);

    tjDestroy(decompressor);
    return 0;
} 

inline int ImageFormatConverter::Converter::webpToJpeg(System::String^ inputPath) {
    std::string path_to_webp = msclr::interop::marshal_as<std::string>(inputPath);

    System::Drawing::Image^ img = this->img_to_convert->Image;
    System::IO::MemoryStream^ memoryStream = gcnew System::IO::MemoryStream();
    img->Save(memoryStream, System::Drawing::Imaging::ImageFormat::Png);

    array<unsigned char>^ managedBytes = memoryStream->ToArray();
    pin_ptr<unsigned char> pinnedBytes = &managedBytes[0];
    unsigned char* bytes = pinnedBytes;

    tjhandle compressor = tjInitCompress();
    if (compressor == NULL) return -1;

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, bytes, managedBytes->Length);
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
        &jpegBuf, &jpegSize, TJSAMP_444, 100, TJFLAG_FASTDCT);
    if (result != 0) {
        tjFree(jpegBuf);
        tjDestroy(compressor);
        return -1;
    }

    std::size_t lastDot = path_to_webp.find_last_of(".");
    std::string outputPath = path_to_webp.substr(0, lastDot) + ".jpg";

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

inline int ImageFormatConverter::Converter::pngToWebp(System::String^ inputPath) {
    std::string path_to_png = msclr::interop::marshal_as<std::string>(inputPath);
    if (path_to_png == "") return -1;

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, path_to_png);
    if (error) return -1;

    unsigned char* output;
    std::size_t image_size = WebPEncodeRGBA(image.data(), width, height, width * 4, 100, &output);
    if (image_size == 0) return -1;

    std::size_t lastDot = path_to_png.find_last_of(".");
    std::string outputPath = path_to_png.substr(0, lastDot) + ".webp";
    FILE* outputFile = fopen(outputPath.c_str(), "wb");
    if (!outputFile) {
        WebPFree(output);
        return -1;
    }

    fwrite(output, image_size, 1, outputFile);
    fclose(outputFile);
    WebPFree(output);

    return 0;
}

inline int ImageFormatConverter::Converter::jpegToWebp(System::String^ inputPath) {
    std::string path_to_png = msclr::interop::marshal_as<std::string>(inputPath);
    if (path_to_png == "") return -1;

    tjhandle decompressor = tjInitDecompress();
    if (decompressor == NULL) return -1;

    std::ifstream file(path_to_png, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        tjDestroy(decompressor);
        return -1;
    }

    std::streamsize size = file.tellg();
    std::vector<unsigned char> buffer(size);
    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        tjDestroy(decompressor);
        file.close();
        return -1;
    }
    file.close();

    int width, height, subsamp, colorspace;
    if (tjDecompressHeader3(decompressor, buffer.data(), size, &width, &height, &subsamp, &colorspace) < 0) {
        tjDestroy(decompressor);
        return -1;
    }

    std::vector<unsigned char> image(width * height * 3);
    if (tjDecompress2(decompressor, buffer.data(), size, image.data(), width, 0, height, TJPF_RGB, 0) < 0) {
        tjDestroy(decompressor);
        return -1;
    }
    tjDestroy(decompressor);

    unsigned char* output;
    std::size_t image_size = WebPEncodeRGB(image.data(), width, height, width * 3, 100, &output);
    if (image_size == 0) return -1;
    
    std::size_t lastDot = path_to_png.find_last_of(".");
    std::string outputPath = path_to_png.substr(0, lastDot) + ".webp";
    FILE* outputFile = fopen(outputPath.c_str(), "wb");
    if (!outputFile) {
        WebPFree(output);
        return -1;
    }

    fwrite(output, image_size, 1, outputFile);
    fclose(outputFile);
    WebPFree(output);

    return 0;
}