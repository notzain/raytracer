cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
if (OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()

include(${CMAKE_BINARY_DIR}/../conanbuildinfo_multi.cmake)
conan_basic_setup(raytracer)


add_executable(raytracer src/main.cpp src/PNG.cpp src/PNG.h src/Ray.cpp src/Ray.h src/Types.h src/Circle.cpp src/Circle.h)
conan_target_link_libraries(raytracer)
