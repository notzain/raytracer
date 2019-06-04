cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/../conanbuildinfo_multi.cmake)
conan_basic_setup()

add_executable(raytracer src/main.cpp src/PNG.cpp src/PNG.h)
conan_target_link_libraries(raytracer)