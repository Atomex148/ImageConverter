#include "Converter.h"

void ImageFormatConverter::Converter::choosePicture(System::Object^ sender, System::EventArgs^ e) {
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();

    openFileDialog->Filter = "JPEG (*.jpg; *.jpeg)|*.jpg;*.jpeg|PNG (*.png)|*.png|WEBP (*.webp)|*.webp";
    openFileDialog->FilterIndex = 1;

    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        filePath = openFileDialog->FileName;
        insertPicture();

        int maxLength = 50;
        String^ displayPath = filePath;

        if (filePath->Length > maxLength) {
            displayPath = filePath->Substring(0, maxLength) + "...";
        }

        this->selectedFiles->Text = displayPath;
        this->toolTip->SetToolTip(this->img_to_convert, this->filePath);
    }
}