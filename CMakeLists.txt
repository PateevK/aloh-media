set(PRJ_NAME "ALOH_MEDIA")

cmake_minimum_required(VERSION 3.25)

project(${PRJ_NAME} CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ALO_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

set(ALO_THIRD_PARTY_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

add_subdirectory(${ALO_THIRD_PARTY_DIR}/spdlog)

file(GLOB_RECURSE ALO_SOURCES CONFIGURE_DEPENDS 
    ${ALO_SRC_DIR}/*.cpp
    )

set(ALO_INCLUDE_DIRS 
${ALO_SRC_DIR}
)

add_executable(${PRJ_NAME} 
    ${ALO_SOURCES}
    ${ALO_THIRD_PARTY_DIR}/miniaudio/miniaudio.c
)

target_include_directories(${PRJ_NAME} PRIVATE 
    ${ALO_INCLUDE_DIRS}
    ${ALO_THIRD_PARTY_DIR}/miniaudio/
)

target_link_libraries(${PRJ_NAME}
    PRIVATE
    spdlog::spdlog
)

target_compile_definitions(${PRJ_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:FUNC_SIG=__FUNCSIG__>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:FUNC_SIG=__PRETTY_FUNCTION__>
)

if (MSVC)
    target_compile_options(${PRJ_NAME} INTERFACE /W4 /WX) # WX treats warnings as errors
else()
    target_compile_options(${PRJ_NAME} INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()