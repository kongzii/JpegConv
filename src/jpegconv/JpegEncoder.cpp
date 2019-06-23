#include "JpegEncoder.hpp"

namespace jpegconv {

JpegEncoder::JpegEncoder() {
    this->enc_info.err = jpeg_std_error(&this->error_mgr);
    jpeg_create_compress(&this->enc_info);
}

JpegEncoder::~JpegEncoder() {
    jpeg_destroy_compress(&this->enc_info);
}

void JpegEncoder::set_color_space(J_COLOR_SPACE color_space, int num_components) {
    this->color_space = color_space;
    this->num_components = num_components;
}

J_COLOR_SPACE JpegEncoder::get_color_space() const {
    return this->color_space;
}

int JpegEncoder::get_num_components() const {
    return this->num_components;
}

void JpegEncoder::set_quality(int quality) {
    this->quality = quality;
}

int JpegEncoder::get_quality() const {
    return quality;
}

bool JpegEncoder::encode(uint8_t* img, int width, int height, FILE* file) {
    if (file == nullptr) {
        return false;
    }

    jpeg_stdio_dest(&this->enc_info, file);
    return encode(img, width, height);
}

bool JpegEncoder::encode(uint8_t* img, int width, int height, const std::string path) {
    FILE* file = fopen(path.c_str(), "wb");
    bool rv = encode(img, width, height, file);
    fclose(file);
    return rv;
}

bool JpegEncoder::encode(uint8_t* img, int width, int height) {
    this->enc_info.image_width = width;
    this->enc_info.image_height = height;
    this->enc_info.in_color_space = this->color_space;
    this->enc_info.input_components = this->num_components;
    jpeg_set_defaults(&this->enc_info);
    jpeg_set_quality(&this->enc_info, quality, true);

    jpeg_start_compress(&this->enc_info, true);

    int rowSize = this->enc_info.image_width * this->enc_info.input_components;
    JSAMPROW rows[1];
    while (this->enc_info.next_scanline < this->enc_info.image_height) {
        rows[0] = img + this->enc_info.next_scanline * rowSize;
        jpeg_write_scanlines(&this->enc_info, rows, 1);
    }

    jpeg_finish_compress(&this->enc_info);
    return true;
}

}
