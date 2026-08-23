cmake_minimum_required(VERSION 4.0)

project(QTip
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

add_library(QTip STATIC
        src/QTip.cpp
        include/Q-Tip/QTip.h
        include/Q-Tip/Config.h
        src/Window/Window.cpp
        include/Q-Tip/Window/Window.h
        src/Graphics/Renderer.cpp
        include/Q-Tip/Graphics/Renderer.h
        include/Q-Tip/Graphics/Color.h
        include/Q-Tip/Graphics/Rect.h
        src/Graphics/Font.cpp
        include/Q-Tip/Graphics/Font.h
)

target_include_directories(QTip
        PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(QTip
    PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)

add_executable(QTipExample
    examples/main.cpp
)

target_include_directories(QTipExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(QTipExample
    PRIVATE
        QTip
)