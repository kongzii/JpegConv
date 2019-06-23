#ifndef JPEGDEC_H_
#define JPEGDEC_H_

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <string>

#include <jpeglib.h>

namespace jpegconv {

class JpegDecoder {
public:
    JpegDecoder();
    virtual ~JpegDecoder();

    void set_color_space(J_COLOR_SPACE color_space);
    J_COLOR_SPACE get_color_space();

    JpegDecoder(const JpegDecoder&) = delete;
    void operator=(const JpegDecoder&) = delete;

    bool decode(uint8_t* buffer, size_t len, uint8_t*& img, int& width, int& height);
    bool decode(FILE* file, uint8_t*& img, int& width, int& height);
    bool decode(const std::string path, uint8_t*& img, int& width, int& height);

private:
    bool decode(uint8_t*& img, int& width, int& height);

    jpeg_decompress_struct dec_info;
    jpeg_error_mgr error_mgr;

    J_COLOR_SPACE color_space = JCS_RGB;
};

}

#endif // JPEGDEC_H_
