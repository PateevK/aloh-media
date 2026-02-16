cmake_minimum_required(VERSION 3.25)
project(ALOH_MEDIA CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ALO_THIRD_PARTY_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

if(NOT TARGET spdlog)
    add_subdirectory(${ALO_THIRD_PARTY_DIR}/spdlog)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} STATIC  
${ALO_THIRD_PARTY_DIR}/miniaudio/miniaudio.c
)

add_library(aloh::media ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
PUBLIC
aloh::media-impl
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${ALO_THIRD_PARTY_DIR}/miniaudio
) 

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:FUNC_SIG=__FUNCSIG__>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:FUNC_SIG=__PRETTY_FUNCTION__>
)

if (MSVC)
    #target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX) # WX treats warnings as errors
else()
    #target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()