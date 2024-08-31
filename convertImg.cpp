#include "Converter.h"

void ImageFormatConverter::Converter::convertImg(System::Object^ sender, System::EventArgs^ e) {
    String^ toFormat = nullptr;

    if (this->formatTo->SelectedItem != nullptr) toFormat = this->formatTo->SelectedItem->ToString();
    else MessageBox::Show(L"Choose format!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);

    switch (current_format) {
    case PNG:
        if (toFormat == L"jpg") pngToJpeg(filePath);
        break;
    case JPG:
        if (toFormat == L"png") jpegToPng(filePath);
        break;
    case BMP:
        break;
    case WEBP:
        if (toFormat == L"png") webpToPng(filePath);
        else if (toFormat == L"jpg") webpToJpeg(filePath);
        break;
    }
}