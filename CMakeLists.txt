cmake_minimum_required(VERSION 3.25)

project("ALOH_MEDIA" CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ALO_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(GLOB_RECURSE ALO_SOURCES CONFIGURE_DEPENDS ${ALO_SRC_DIR}/*.cpp)

set(ALO_INCLUDE_DIRS 
${ALO_SRC_DIR}
)

add_executable("ALO" ${ALO_SOURCES})

target_include_directories("ALO" PRIVATE 
    ${ALO_INCLUDE_DIRS}
)

target_link_libraries("ALO"
    PRIVATE

)