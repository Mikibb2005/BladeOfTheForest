cmake_minimum_required(VERSION 3.10)
project(BladeForest)

set(CMAKE_CXX_STANDART 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(BladeForest src/main.cpp)
target_link_libraries(BladeForest ${SDL2_LIBRARIES})