# Image Converter

A simple Windows desktop application for converting between various image formats.

## Features

- **Multiple Format Conversions**:
  - PNG to JPEG
  - PNG to WebP
  - JPEG to PNG
  - JPEG to WebP
  - WebP to PNG
  - WebP to JPEG
- **User-Friendly Interface**: Built with Windows Forms for ease of use.
- **Drag-and-Drop**: Drag image files directly into the application window to load them.
- **File Picker**: Alternatively, click to open a standard file dialog to select an image.
- **Image Preview**: Displays the selected image before conversion.
- **Two UI Modes**:
    - **Maximum Mode**: Full interface with an image preview area.
    - **Minimum Mode**: A compact interface without the image preview, ideal for quick conversions.

## Technical Details

- **Language**: C++/CLI
- **Framework**: .NET Framework (Windows Forms)
- **Libraries**:
    - **libjpeg-turbo**: Used for high-speed JPEG compression and decompression.
    - **libwebp**: The official Google library for encoding and decoding WebP images.
    - **LodePNG**: A lightweight library for handling PNG file encoding and decoding.

## How to Use

1.  Launch the `ImageFormatConverter.exe` application.
2.  Select an image for conversion using one of the following methods:
    - **Drag and Drop**: Drag an image file (PNG, JPG, BMP, or WebP) onto the preview area.
    - **Click to Select**: Click on the preview area (or the "Choose picture" button in Minimum mode) to open a file selection dialog.
3.  Choose the desired output format from the dropdown menu.
4.  Click the `CONVERT` button.
5.  The new image file will be created in the same directory as the original file, with the appropriate file extension.

## Building from Source

### Prerequisites

- Visual Studio with the "Desktop development with C++" workload.
- **libjpeg-turbo**: This library is not included in the repository. You must download it separately and place it in a location accessible to the project.

### Steps

1.  Clone the repository:
    ```bash
    git clone https://github.com/atomex148/ImageConverter.git
    ```
2.  Download and extract **libjpeg-turbo** for your system architecture.

3.  Open the `ImageFormatConverter.sln` solution file in Visual Studio.

4.  The project file (`.vcxproj`) contains hardcoded paths to the required libraries. You will need to update them.
    -   In the Solution Explorer, right-click on the `ImageFormatConverter` project and select **Properties**.
    -   Go to **Configuration Properties** -> **VC++ Directories**.
    -   Edit **Include Directories** to add the path to the `include` folder of `libjpeg-turbo`.
    -   Edit **Library Directories** to add the path to the `lib` folder of `libjpeg-turbo`.
    -   Go to **Configuration Properties** -> **Linker** -> **Input**.
    -   Edit **Additional Dependencies** to ensure the paths to `libwebp.lib`, `libwebpdemux.lib`, `libwebpmux.lib`, and `turbojpeg.lib` are correct for your system. The `libwebp` libraries are located within the `libwebp-1.4.0-windows-x64` directory in this repository.

5.  Build the solution (Build -> Build Solution). The executable will be created in the `x64/Debug` or `x64/Release` folder inside the project directory.
