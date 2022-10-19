cmake_minimum_required(VERSION 2.8)
project(draw)

set(CMAKE_BUILD_TYPE "Release")
add_definitions("-DENABLE_SSE")
set(CMAKE_CXX_FLAGS "-std=c++11 ${SSE_FLAGS} -g -O3 -march=native")
include_directories("/usr/include/eigen3")
find_package(OpenCV 4 REQUIRED)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(draw draw.cpp)
target_link_libraries(draw ${OpenCV_LIBS})
