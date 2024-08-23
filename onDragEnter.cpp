#include "Converter.h"

void ImageFormatConverter::Converter::onDragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e) {
    if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
        array<String^>^ files = dynamic_cast<array<String^>^>(e->Data->GetData(DataFormats::FileDrop));
        notImg = false;

        for (int i = 0; i < files->Length; i++) {
            String^ filePath = files[i];
            int dotIndex = filePath->LastIndexOf(L'.');
            if (dotIndex != -1) {
                String^ extension = filePath->Substring(dotIndex + 1);
                if (extension->ToLower() != L"png" && extension->ToLower() != L"jpg" &&
                    extension->ToLower() != L"jpeg" && extension->ToLower() != L"webp" &&
                    extension->ToLower() != L"bmp")
                {
                    notImg = true;
                    break;
                }
            }
        }
        
        if (notImg) e->Effect = DragDropEffects::None;
        else e->Effect = DragDropEffects::Copy;
    }
    else e->Effect = DragDropEffects::None;
}