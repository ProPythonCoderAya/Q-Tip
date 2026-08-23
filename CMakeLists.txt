cmake_minimum_required(VERSION 4.0)
project(QTip)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(QTip
        main.cpp
)

include_directories("src")

target_link_libraries(QTip PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)
