cmake_minimum_required(VERSION 3.14)
project(JpegConv)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

find_package(JPEG REQUIRED)

file(GLOB jpegconv_src
    "src/jpegconv/*.hpp"
    "src/jpegconv/*.cpp"
)

add_executable(JpegConv src/main.cpp ${jpegconv_src})

target_link_libraries(JpegConv ${JPEG_LIBRARY})