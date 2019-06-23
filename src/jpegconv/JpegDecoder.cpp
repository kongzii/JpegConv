#include "JpegDecoder.hpp"

namespace jpegconv {

JpegDecoder::JpegDecoder() {
    this->dec_info.err = jpeg_std_error(&this->error_mgr);
    jpeg_create_decompress(&this->dec_info);
}

JpegDecoder::~JpegDecoder() {
    jpeg_destroy_decompress(&this->dec_info);
}

void JpegDecoder::set_color_space(J_COLOR_SPACE color_space) {
    this->color_space = color_space;
}

J_COLOR_SPACE JpegDecoder::get_color_space() {
    return this->color_space;
}

bool JpegDecoder::decode(uint8_t* buffer, size_t len, uint8_t*& img, int& width, int& height) {
    if (buffer == nullptr) {
        return false;
    }

    jpeg_mem_src(&this->dec_info, buffer, len);
    return decode(img, width, height);
}

bool JpegDecoder::decode(FILE* file, uint8_t*& img, int& width, int& height) {
    if (file == nullptr) {
        return false;
    }

    jpeg_stdio_src(&this->dec_info, file);
    return decode(img, width, height);
}

bool JpegDecoder::decode(const std::string path, uint8_t*& img, int& width, int& height) {
    FILE* file = fopen(path.c_str(), "rb");
    bool rv = decode(file, img, width, height);
    fclose(file);
    return rv;
}

bool JpegDecoder::decode(uint8_t*& img, int& width, int& height) {
    jpeg_read_header(&this->dec_info, true);
    this->dec_info.out_color_space = this->color_space;
    this->dec_info.raw_data_out = false;

    jpeg_start_decompress(&this->dec_info);

    width = this->dec_info.image_width;
    height = this->dec_info.image_height;
    size_t rowSize = this->dec_info.image_width * this->dec_info.num_components;
    img = new uint8_t[this->dec_info.image_height * rowSize];

    JSAMPROW rows[1];
    size_t offset = 0;
    while (this->dec_info.output_scanline < this->dec_info.image_height) {
        rows[0] = img + offset;
        jpeg_read_scanlines(&this->dec_info, rows, 1);
        offset += rowSize;
    }

    jpeg_finish_decompress(&this->dec_info);
    return true;
}

}
