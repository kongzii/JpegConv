//
// Created by peter on 23/06/19.
//

#ifndef IMAGEPOOLING_JPEGCONV_HPP
#define IMAGEPOOLING_JPEGCONV_HPP

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <memory>

#include <jpeglib.h>

#include "JpegDecoder.hpp"
#include "JpegEncoder.hpp"

namespace jpegconv {

    typedef struct {
        uint8_t *pixels;

        int width;
        int height;
    } image_t;

    enum PoolingVariant {
        MaxPooling, MinPooling
    };

    class JpegConv {
    public:
        static const double edge_filter[3][3];
        static const double blur_filter[3][3];
        static const double sharpen_filter[3][3];

        JpegConv(std::string filename, J_COLOR_SPACE color_space);

        explicit JpegConv(std::string filename) : JpegConv(filename, JCS_RGB) {};

        void load(std::string filename);

        void save(std::string filename);

        // Filters

        void apply_filter(const double filter[3][3]);

        void apply_pooling(const int dimension);

        void apply_pooling(const int dimension, const PoolingVariant variant);

        // Getters

        J_COLOR_SPACE get_color_space() { return this->color_space; }

    private:
        image_t input_image;
        image_t output_image;

        J_COLOR_SPACE color_space;

        double normalize_pixel(double p);

        uint8_t pool_compare(uint8_t a, uint8_t b, PoolingVariant variant);

    };

}

#endif //IMAGEPOOLING_JPEGCONV_HPP