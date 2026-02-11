# ------------------------------------------------------------------
# 1. Compiler Selection (MUST BE BEFORE PROJECT)
# ------------------------------------------------------------------
if(WIN32)
    # --- WINDOWS (MinGW-Clang) ---
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)

    # Force Clang acting as MinGW
    set(CMAKE_C_COMPILER "D:/someProgs/msys2/clang64/bin/clang.exe")
    set(CMAKE_CXX_COMPILER "D:/someProgs/msys2/clang64/bin/clang++.exe")

    # Crucial Flags for MinGW
    set(CMAKE_C_FLAGS "--target=x86_64-w64-mingw32")
    set(CMAKE_CXX_FLAGS "--target=x86_64-w64-mingw32")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

else()
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
endif()