# JpegConv
Convolutional layer for JPEG images. Load image, apply filters, save.

Supports sharpen, edge detection and blur filters by default. Image pooling is also implemented to reduce final resolution. 

Custom filter matrices can be supplied to apply_filter method.

## Example

Original image on left, right image is result of applying sharpen > edge detection > blur filter and pooling by 3x3 window.

![Example image](https://github.com/kongzii/JpegConv/blob/master/sample.png "Example image")

### Source code

```c++
#include "jpegconv/JpegConv.hpp"

using namespace jpegconv;

int main(int argc, char **argv) {
    const std::string input_file = "tobias.jpg";
    const std::string output_file = "output.jpg";

    // Read

    auto jpegconv = JpegConv(input_file);

    // Apply sharpen filter

    jpegconv.apply_filter(JpegConv::sharpen_filter);

    // Apply edge detection filter

    jpegconv.apply_filter(JpegConv::edge_filter);

    // Apply blur filter

    jpegconv.apply_filter(JpegConv::blur_filter);

    // Resize, max pooling by default

    jpegconv.apply_pooling(3);

    // Apply custom filter

    /*
        const double custom[3][3] = {
                { 0, -1,  0},
                {-1,  5, -1},
                { 0, -1,  0}
        };

        jpegconv.apply_filter(custom);
    */

    // Save

    jpegconv.save(output_file);
}

```