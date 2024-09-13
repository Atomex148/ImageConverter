#include "Converter.h"

void ImageFormatConverter::Converter::onDragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e) {
    if (e->Data->GetDataPresent(DataFormats::FileDrop) && !notImg) {
        array<String^>^ files = dynamic_cast<array<String^>^>(e->Data->GetData(DataFormats::FileDrop));
        filePath = files[0];

        if (this->img_to_convert->Image != nullptr) {
            delete this->img_to_convert->Image;
            this->img_to_convert->Image = nullptr;
        }

        insertPicture();
        this->toolTip->SetToolTip(this->img_to_convert, this->filePath);
    }
}