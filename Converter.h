#pragma once
#include "lodepng.h"
#include <msclr/marshal_cppstd.h>
#include <string>
#include <fstream>
#include <webp/decode.h>
#include <webp/encode.h>
#include <turbojpeg.h>

enum CURRENT_IMG_FORMAT {
    IMG_NULL = 0,
    PNG,
    JPG,
    WEBP,
    BMP
};

namespace ImageFormatConverter {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Converter : public System::Windows::Forms::Form
    {
    public:
        Converter(void)
        {
            InitializeComponent();
            this->AllowDrop = true;
            formatNamesDefault = gcnew array<System::String^> { L"png", L"jpg", L"bmp", L"webp" };
            formatNames = gcnew array<System::String^> {};
            formatTo->Items->AddRange(formatNamesDefault);
            this->DragEnter += gcnew DragEventHandler(this, &Converter::onDragEnter);
            this->DragDrop += gcnew DragEventHandler(this, &Converter::onDragDrop);
        }

    protected:
        ~Converter()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        array<System::String^>^ formatNamesDefault, ^formatNames;
        System::Windows::Forms::ComboBox^ formatTo;
        System::Windows::Forms::Button^ convert_btn;
        System::Windows::Forms::PictureBox^ img_to_convert;
        System::ComponentModel::Container^ components;
        CURRENT_IMG_FORMAT current_format = IMG_NULL;
        String^ filePath;

        bool notImg = false;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->formatTo = (gcnew System::Windows::Forms::ComboBox());
            this->convert_btn = (gcnew System::Windows::Forms::Button());
            this->img_to_convert = (gcnew System::Windows::Forms::PictureBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->img_to_convert))->BeginInit();
            this->SuspendLayout();
            // 
            // formatTo
            // 
            this->formatTo->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->formatTo->FormattingEnabled = true;
            this->formatTo->Location = System::Drawing::Point(12, 356);
            this->formatTo->Name = L"formatTo";
            this->formatTo->Size = System::Drawing::Size(460, 21);
            this->formatTo->TabIndex = 1;
            // 
            // convert_btn
            // 
            this->convert_btn->Location = System::Drawing::Point(32, 394);
            this->convert_btn->Name = L"convert_btn";
            this->convert_btn->Size = System::Drawing::Size(419, 55);
            this->convert_btn->TabIndex = 2;
            this->convert_btn->Text = L"CONVERT";
            this->convert_btn->UseVisualStyleBackColor = true;
            this->convert_btn->Click += gcnew System::EventHandler(this, &Converter::convertImg);
            // 
            // img_to_convert
            // 
            this->img_to_convert->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->img_to_convert->Cursor = System::Windows::Forms::Cursors::Default;
            this->img_to_convert->Location = System::Drawing::Point(12, 12);
            this->img_to_convert->Name = L"img_to_convert";
            this->img_to_convert->Size = System::Drawing::Size(460, 323);
            this->img_to_convert->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->img_to_convert->TabIndex = 3;
            this->img_to_convert->TabStop = false;
            //this->img_to_convert->Click += gcnew System::EventHandler(this, &Converter::img_to_convert_Click);
            // 
            // Converter
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(484, 461);
            this->Controls->Add(this->img_to_convert);
            this->Controls->Add(this->convert_btn);
            this->Controls->Add(this->formatTo);
            this->Name = L"Converter";
            this->Text = L"Converter";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->img_to_convert))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
    private: 
        void convertImg(System::Object^ sender, System::EventArgs^ e);
        void onDragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
        void onDragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
        std::vector<unsigned char> webpToPng(System::String^ path);
        int pngToJpeg(System::String^ path);
        int jpegToPng(System::String^ path);
        void choosePicture(System::Object^ sender, System::EventArgs^ e) {}
    };
}
