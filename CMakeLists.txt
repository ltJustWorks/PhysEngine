cmake_minimum_required(VERSION 3.26)
project(PhysEngine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2-2.28.5/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(PhysEngine main.cpp)

target_link_libraries(PhysEngine ${SDL2_LIBRARY})