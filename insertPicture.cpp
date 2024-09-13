#include "Converter.h"

void ImageFormatConverter::Converter::insertPicture() {
    if (this->img_to_convert->Image != nullptr) {
        delete this->img_to_convert->Image;  
        this->img_to_convert->Image = nullptr;
    }

    int dotIndex = filePath->LastIndexOf(L'.');
    String^ extension = filePath->Substring(dotIndex + 1)->ToLower();
    Generic::List<String^>^ newFormatList = gcnew System::Collections::Generic::List<String^>();

    if (extension == L"jpeg" || extension == L"jpg") {
        extension = L"jpg";
        current_format = JPG;
        this->img_to_convert->Image = System::Drawing::Image::FromFile(filePath);
    }
    else if (extension == L"webp") {
        std::vector<unsigned char> png_data = webpToPng_forProgramImage(filePath);
        if (!png_data.empty()) {
            cli::array<unsigned char>^ pngArray = gcnew cli::array<unsigned char>(png_data.size());
            System::Runtime::InteropServices::Marshal::Copy((IntPtr)png_data.data(), pngArray, 0, png_data.size());
            System::IO::MemoryStream^ ms = gcnew System::IO::MemoryStream(pngArray);
            try {
                System::Drawing::Image^ image = System::Drawing::Image::FromStream(ms);
                this->img_to_convert->Image = image;
            }
            catch (System::Exception^ e) {
                MessageBox::Show("Error creating image: " + e->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
        else {
            MessageBox::Show("Failed to load WebP", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        current_format = WEBP;
    }
    else if (extension == L"png") {
        current_format = PNG;
        this->img_to_convert->Image = System::Drawing::Image::FromFile(filePath);
    }
    else if (extension == L"bmp") {
        current_format = BMP;
        this->img_to_convert->Image = System::Drawing::Image::FromFile(filePath);
    }

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