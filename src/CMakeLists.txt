set(ALOH_MEDIA_IMPL "aloh-media-impl")

set(ALO_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ALO_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB_RECURSE ALO_SOURCES CONFIGURE_DEPENDS 
    ${ALO_SRC_DIR}/*.cpp
    
)

add_library(${ALOH_MEDIA_IMPL} OBJECT ${ALO_SOURCES})
add_library(aloh::media-impl ALIAS ${ALOH_MEDIA_IMPL})

target_link_libraries(${ALOH_MEDIA_IMPL}
    PUBLIC
    spdlog::spdlog
)

target_include_directories(${ALOH_MEDIA_IMPL} PUBLIC 
    ${ALO_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/miniaudio
)

# Fancy macro
if(MSVC)
    # Visual Studio
    set(SIG_INTRINSIC "__FUNCSIG__")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # GCC or Clang (Unix/Linux/Mac)
    set(SIG_INTRINSIC "__PRETTY_FUNCTION__")
else()
    # Fallback for Standard C++11 / C99
    set(SIG_INTRINSIC "__func__")
endif()

target_compile_definitions(${ALOH_MEDIA_IMPL} PUBLIC FUNC_SIG=${SIG_INTRINSIC})
