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
