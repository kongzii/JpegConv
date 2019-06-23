//
// Created by peter on 23/06/19.
//

#include "JpegConv.hpp"

namespace jpegconv {

    JpegConv::JpegConv(std::string filename, J_COLOR_SPACE color_space) {
        this->color_space = color_space;
        this->load(filename);
    }

    void JpegConv::load(std::string filename) {
        auto decoder = new JpegDecoder();

        decoder->set_color_space(this->color_space);

        decoder->decode(filename, this->input_image.pixels, this->input_image.width, this->input_image.height);
        decoder->decode(filename, this->output_image.pixels, this->output_image.width, this->output_image.height);

        delete decoder;
    }

    void JpegConv::save(std::string filename) {
        auto encoder = std::make_unique<JpegEncoder>();

        encoder->set_color_space(this->color_space, 3);

        encoder->encode(this->output_image.pixels, this->output_image.width, this->output_image.height, filename);
    }

    double JpegConv::normalize_pixel(double p) {
        if (p > 255) {
            return 255;
        } else if (p < 0) {
            return 0;
        } else {
            return p;
        }
    }

    uint8_t JpegConv::pool_compare(uint8_t a, uint8_t b, PoolingVariant variant) {
        switch (variant) {
            case MaxPooling:
                return a > b ? a : b;
            case MinPooling:
                return a < b ? a : b;
        }
    }

    void JpegConv::apply_filter(const double filter[3][3]) {
        auto image_filtered = image_t{new uint8_t[this->output_image.width * this->output_image.height * 3],
                                    this->output_image.width, this->output_image.height};

        for (int y = 0; y < image_filtered.height; y++) {
            int row_conv = image_filtered.width * y * 3;

            for (int x = 0; x < image_filtered.width; x++) {
                int offset_conv = row_conv + x * 3;

                double a = 0, b = 0, c = 0;

                int y3 = 0;
                for (int y2 = y - 1; y2 <= y + 1; y2++) {
                    int row_org = this->output_image.width * y2 * 3;

                    int x3 = 0;
                    for (int x2 = x - 1; x2 <= x + 1; x2++) {
                        int offset_org = row_org + x2 * 3;

                        a += (double) this->output_image.pixels[offset_org] * filter[y3][x3];
                        b += (double) this->output_image.pixels[offset_org + 1] * filter[y3][x3];
                        c += (double) this->output_image.pixels[offset_org + 2] * filter[y3][x3];

                        x3++;
                    }

                    y3++;
                }

                image_filtered.pixels[offset_conv] = (uint8_t) normalize_pixel(a);
                image_filtered.pixels[offset_conv + 1] = (uint8_t) normalize_pixel(b);
                image_filtered.pixels[offset_conv + 2] = (uint8_t) normalize_pixel(c);
            }
        }

        this->output_image = image_filtered;
    }

    void JpegConv::apply_pooling(const int dimension) {
        return this->apply_pooling(dimension, MaxPooling);
    }

    void JpegConv::apply_pooling(const int dimension, const PoolingVariant variant) {
        int width_conv = this->output_image.width / dimension;
        int height_conv = this->output_image.height / dimension;

        auto image_pool = image_t{new uint8_t[width_conv * height_conv * 3], width_conv, height_conv};

        for (int y = 0; y < image_pool.height; y++) {
            int row_conv = image_pool.width * y * 3;

            for (int x = 0; x < image_pool.width; x++) {
                int offset_conv = row_conv + x * 3;

                uint8_t default_val;

                switch (variant) {
                    case MaxPooling:
                        default_val = std::numeric_limits<uint8_t >::min();
                        break;
                    case MinPooling:
                        default_val = std::numeric_limits<uint8_t >::max();
                        break;
                }

                uint8_t max_a = default_val, max_b = default_val, max_c = default_val;

                for (int y2 = y * dimension; y2 < y * dimension + dimension; y2++) {
                    int row_org = this->output_image.width * y2 * 3;

                    for (int x2 = x * dimension; x2 < x * dimension + dimension; x2++) {
                        int offset_org = row_org + x2 * 3;

                        max_a = pool_compare(max_a, this->output_image.pixels[offset_org], variant);
                        max_b = pool_compare(max_b, this->output_image.pixels[offset_org + 1], variant);
                        max_c = pool_compare(max_c, this->output_image.pixels[offset_org + 2], variant);
                    }
                }

                image_pool.pixels[offset_conv] = max_a;
                image_pool.pixels[offset_conv + 1] = max_b;
                image_pool.pixels[offset_conv + 2] = max_c;
            }
        }

        this->output_image = image_pool;
    }

}