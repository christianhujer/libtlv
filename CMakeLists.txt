cmake_minimum_required(VERSION 3.16)
project(libtlv LANGUAGES C VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)

add_library(tlv src/tlv.c include/tlv.h)

add_executable(libtlv_test test/test_main.c test/tlv_test.c test/tlv_test.h)
target_link_libraries(libtlv_test tlv)
include_directories(include)
