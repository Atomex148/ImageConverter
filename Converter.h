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
            this->img_to_convert->Click += gcnew System::EventHandler(this, &Converter::choosePicture);
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
        ComboBox^ formatTo;
        Button^ convert_btn;
        PictureBox^ img_to_convert;
        System::ComponentModel::Container^ components;
        CURRENT_IMG_FORMAT current_format = IMG_NULL;
        String^ filePath = L"";
        System::Windows::Forms::MenuStrip^ menuStrip1;
        System::Windows::Forms::ToolStripMenuItem^ programFormatToolStripMenuItem, ^ minimumMenuItem, ^ maximumMenuItem;
        System::Windows::Forms::Label^ selectedFiles;
        System::Windows::Forms::ToolTip^ toolTip;
        
        bool notImg = false;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->formatTo = (gcnew System::Windows::Forms::ComboBox());
            this->convert_btn = (gcnew System::Windows::Forms::Button());
            this->img_to_convert = (gcnew System::Windows::Forms::PictureBox());
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->programFormatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->minimumMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->maximumMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->img_to_convert))->BeginInit();
            this->selectedFiles = (gcnew System::Windows::Forms::Label());
            this->toolTip = (gcnew System::Windows::Forms::ToolTip());
            this->menuStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // formatTo
            // 
            this->formatTo->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->formatTo->FormattingEnabled = true;
            this->formatTo->Location = System::Drawing::Point(12, 386);
            this->formatTo->Name = L"formatTo";
            this->formatTo->Size = System::Drawing::Size(460, 21);
            this->formatTo->TabIndex = 1;
            // 
            // convert_btn
            // 
            this->convert_btn->Location = System::Drawing::Point(32, 424);
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
            this->img_to_convert->Location = System::Drawing::Point(12, 42);
            this->img_to_convert->Name = L"img_to_convert";
            this->img_to_convert->Size = System::Drawing::Size(460, 323);
            this->img_to_convert->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->img_to_convert->TabIndex = 3;
            this->img_to_convert->TabStop = false;
            // 
            // menuStrip1
            // 
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->programFormatToolStripMenuItem });
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(484, 24);
            this->menuStrip1->TabIndex = 4;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // programFormatToolStripMenuItem
            // 
            this->programFormatToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->minimumMenuItem,
                    this->maximumMenuItem
            });
            this->programFormatToolStripMenuItem->Name = L"programFormatToolStripMenuItem";
            this->programFormatToolStripMenuItem->Size = System::Drawing::Size(104, 20);
            this->programFormatToolStripMenuItem->Text = L"Program format";
            // 
            // minimumMenuItem
            // 
            this->minimumMenuItem->Name = L"minimumMenuItem";
            this->minimumMenuItem->Size = System::Drawing::Size(129, 22);
            this->minimumMenuItem->Text = L"Minimum";
            this->minimumMenuItem->Click += gcnew System::EventHandler(this, &Converter::minimumMenuChoice_Click);
            // 
            // maximumMenuItem
            // 
            this->maximumMenuItem->Checked = true;
            this->maximumMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
            this->maximumMenuItem->Name = L"maximumMenuItem";
            this->maximumMenuItem->Size = System::Drawing::Size(129, 22);
            this->maximumMenuItem->Text = L"Maximum";
            this->maximumMenuItem->Click += gcnew System::EventHandler(this, &Converter::maximumMenuChoice_Click);
            // 
            // selectedFiles
            // 
            this->selectedFiles->Visible = false;
            this->selectedFiles->Location = System::Drawing::Point(150, 48);
            this->selectedFiles->Text = filePath;
            this->selectedFiles->AutoSize = true;
            // 
            // toolTip
            // 
            this->toolTip->SetToolTip(this->img_to_convert, this->filePath);
            // 
            // Converter
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(484, 491);
            this->Controls->Add(this->img_to_convert);
            this->Controls->Add(this->convert_btn);
            this->Controls->Add(this->formatTo);
            this->Controls->Add(this->menuStrip1);
            this->Controls->Add(selectedFiles);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"Converter";
            this->Text = L"Converter";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->img_to_convert))->EndInit();
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();
        }
#pragma endregion
    private: 
        bool isMaximumMode;
        void convertImg(System::Object^ sender, System::EventArgs^ e);
        void onDragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
        void onDragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
        inline std::vector<unsigned char> webpToPng_forProgramImage(System::String^ path);
        inline int webpToPng(System::String^ path);
        inline int pngToJpeg(System::String^ path);
        inline int jpegToPng(System::String^ path);
        inline int webpToJpeg(System::String^ path);
        inline int jpegToWebp(System::String^ path);
        inline int pngToWebp(System::String^ path);
        void choosePicture(System::Object^ sender, System::EventArgs^ e);
        void insertPicture();

        void minimumMenuChoice_Click(System::Object^ sender, System::EventArgs^ e) {
            this->minimumMenuItem->Checked = true;
            this->maximumMenuItem->Checked = false;
            this->img_to_convert->AllowDrop = false;

            this->img_to_convert->Click -= gcnew System::EventHandler(this, &Converter::choosePicture);
            this->DragEnter -= gcnew DragEventHandler(this, &Converter::onDragEnter);
            this->DragDrop -= gcnew DragEventHandler(this, &Converter::onDragDrop);

            if (this->img_to_convert->Image != nullptr) {
                delete this->img_to_convert->Image;
                this->img_to_convert->Image = nullptr;
            }

            this->img_to_convert->Visible = false;
            this->ClientSize = System::Drawing::Size(484, 191);
            this->formatTo->Location = System::Drawing::Point(12, 86);
            this->convert_btn->Location = System::Drawing::Point(32, 124);

            System::Windows::Forms::Button^ choosePicture_btn = gcnew System::Windows::Forms::Button();
            choosePicture_btn->Text = "Choose picture";
            choosePicture_btn->Size = System::Drawing::Size(130, 30);
            choosePicture_btn->Location = System::Drawing::Point(12, 40);
            choosePicture_btn->Click += gcnew System::EventHandler(this, &Converter::choosePicture);
            
            this->Controls->Add(choosePicture_btn);

            this->selectedFiles->Visible = true;

            GC::Collect();
            GC::WaitForPendingFinalizers();
        }

        void maximumMenuChoice_Click(System::Object^ sender, System::EventArgs^ e) {
            this->maximumMenuItem->Checked = true;
            this->minimumMenuItem->Checked = false;
            isMaximumMode = true;
            this->img_to_convert->AllowDrop = true; 

            this->img_to_convert->Click += gcnew System::EventHandler(this, &Converter::choosePicture);
            this->DragEnter += gcnew DragEventHandler(this, &Converter::onDragEnter);
            this->DragDrop += gcnew DragEventHandler(this, &Converter::onDragDrop);

            this->img_to_convert->Visible = true;
            this->ClientSize = System::Drawing::Size(484, 491);
            this->formatTo->Location = System::Drawing::Point(12, 386);
            this->convert_btn->Location = System::Drawing::Point(32, 424);
        
            this->selectedFiles->Visible = false;

            if (!String::IsNullOrEmpty(filePath)) {
                insertPicture();
            }

            GC::Collect();
            GC::WaitForPendingFinalizers();
        }
};
}
