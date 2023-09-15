# Document Scanner Using OpenCV

This is a document scanner application using OpenCV in C++. It can process an image, detect the largest document-like shape, and then perform perspective transformation (warping) and cropping to extract the document.

## Table of Contents

- [Demo](#demo)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Demo

![Original Image](Images/doc-img.jpg)
![Dilated Image](Images/doc-img-dilated.png)
![Warped Image](Images/doc-img-warped.png)
![Cropped Image](Images/doc-img-cropped.png)

## Prerequisites

Before you begin, ensure you have met the following requirements:

- [OpenCV](https://opencv.org/) installed on your system.
- CMake (for building C++ code).

## Installation

1. Clone the repository:
```shell
git clone https://github.com/yourusername/document-scanner.git
```

2. Build the project using CMake:
```shell
cd document-scanner
mkdir build
cd build
cmake ..
make
```
3. Usage
Replace the Resources/doc-img.jpg file with your own image.

4. Run the application:
```shell
./document-scanner
```

5. The application will display the original image, dilated image, warped image, and cropped image.

## Contributing
Contributions are welcome! If you have any improvements or bug fixes, please submit a pull request.

## License
This project will soon be licensed under the MIT License - see the LICENSE file for details.
