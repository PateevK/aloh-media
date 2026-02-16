cmake_minimum_required(VERSION 3.25)
project(ALOH_MEDIA_EXAMPLE CXX C)

add_executable(example main.cpp)
target_link_libraries(example aloh::media)