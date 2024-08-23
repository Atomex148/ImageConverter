#include "Converter.h"

void ImageFormatConverter::Converter::convertImg(System::Object^ sender, System::EventArgs^ e) {
    String^ toFormat = nullptr;

    if (this->formatTo->SelectedItem != nullptr)  toFormat = this->formatTo->SelectedItem->ToString();
    else MessageBox::Show(L"Choose format!", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
}