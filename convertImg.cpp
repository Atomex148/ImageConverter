#include "Converter.h"

void ImageFormatConverter::Converter::convertImg(System::Object^ sender, System::EventArgs^ e) {
    String^ toFormat = nullptr;

    if (this->formatTo->SelectedItem != nullptr) toFormat = this->formatTo->SelectedItem->ToString();
    else MessageBox::Show(L"Choose format!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);

    switch (current_format) {
    case PNG:
        if (toFormat == L"jpg") {
            if (pngToJpeg(filePath) != 0) MessageBox::Show(L"PNG to JPEG conversion has been failed!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            break;
        }
        else if (toFormat == L"webp") {
            if (pngToWebp(filePath) != 0) MessageBox::Show(L"PNG to WEBP conversion has been failed!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            break;
        }
        break;
    case JPG:
        if (toFormat == L"png") {
            if (jpegToPng(filePath) != 0) MessageBox::Show(L"JPEG to PNG conversion has been failed!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            break;
        }
        if (toFormat == L"webp") {
            if (jpegToWebp(filePath) != 0) MessageBox::Show(L"JPEG to WEBP conversion has been failed!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            break;
        }
        break;
    case BMP:
        break;
    case WEBP:
        if (toFormat == L"png") {
            if (webpToPng(filePath) != 0) MessageBox::Show(L"WEBP to PNG conversion has been failed!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            break;
        }
        else if (toFormat == L"jpg") {
            if (webpToJpeg(filePath) != 0) MessageBox::Show(L"JPEG to PNG conversion has been failed!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            break;
        }
        break;
    }
}