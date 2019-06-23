//
// Created by peter on 23/06/19.
//

#include "JpegConv.hpp"

namespace jpegconv {

const double JpegConv::edge_filter[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
};

const double JpegConv::blur_filter[3][3] = {
        {1 / 9.0, 1 / 9.0, 1 / 9.0},
        {1 / 9.0, 1 / 9.0, 1 / 9.0},
        {1 / 9.0, 1 / 9.0, 1 / 9.0}
};

const double JpegConv::sharpen_filter[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
};

}
