#ifndef JPEGENC_H_
#define JPEGENC_H_

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <jpeglib.h>

namespace jpegconv {

class JpegEncoder {
public:
    JpegEncoder();
    virtual ~JpegEncoder();

    JpegEncoder(const JpegEncoder&) = delete;
    void operator=(const JpegEncoder&) = delete;

    void set_color_space(J_COLOR_SPACE colorSpace, int numComponents);
    J_COLOR_SPACE get_color_space() const;

    int get_num_components() const;

    void set_quality(int quality);

    int get_quality() const;

    bool encode(uint8_t* img, int width, int height, FILE* file);

    bool encode(uint8_t* img, int width, int height, const std::string path);

private:
    bool encode(uint8_t* img, int width, int height);

    jpeg_compress_struct enc_info;
    jpeg_error_mgr error_mgr;

    J_COLOR_SPACE color_space = JCS_RGB;
    int num_components = 3;

    int quality = 80;
};

}

#endif // JPEGENC_H_
