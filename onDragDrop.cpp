#include "Converter.h"

void ImageFormatConverter::Converter::onDragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e) {
    if (e->Data->GetDataPresent(DataFormats::FileDrop) && !notImg) {
        array<String^>^ files = dynamic_cast<array<String^>^>(e->Data->GetData(DataFormats::FileDrop));
        String^ filePath = files[0];
        this->img_to_convert->Image = Image::FromFile(filePath);

        int dotIndex = filePath->LastIndexOf(L'.');
        String^ extension = filePath->Substring(dotIndex + 1)->ToLower();
        Generic::List<String^>^ newFormatList = gcnew System::Collections::Generic::List<String^>();

        if (extension == L"jpeg") extension = L"jpg";
        for each (String ^ format in formatNamesDefault) {
            if (!format->ToLower()->Contains(extension)) {
                newFormatList->Add(format);
            }
        }

        formatNames = newFormatList->ToArray();
        formatTo->Items->Clear();
        formatTo->Items->AddRange(formatNames);

        if (formatTo->Items->Count > 0) {
            formatTo->SelectedIndex = 0;
        }
    }
}