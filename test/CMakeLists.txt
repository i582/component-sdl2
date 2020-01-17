cmake_minimum_required(VERSION 3.14)
project(component_sdl2_tests)

set(PROJECT_NAME component_sdl2_tests)

set(CMAKE_CXX_STANDARD 17)


set(SOURCES
    test-utils.cpp
    test-extended-point.cpp)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} component_sdl2_cmake)


# Set up SDL2
set(SDL2_PATH "C:\\SDL2")
set(SDL2_IMAGE_PATH "C:\\SDL2\\SDL2_image-2.0.4")
set(SDL2_TTF_PATH "C:\\SDL2\\SDL2_ttf-2.0.15")
set(SDL2_GFX_PATH "C:\\SDL2\\SDL2_gfx")

# Find SDL2, SDL2_image and SDL2_gfx libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)

# Link SDL2::Main, SDL2::Image and SDL2::GFX to our project
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF SDL2::GFX)