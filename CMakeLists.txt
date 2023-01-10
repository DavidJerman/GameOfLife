cmake_minimum_required(VERSION 3.22)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)

# Set source files
set(SOURCE_FILES
        main.cpp
        game.cpp)

# Set header files
set(HEADER_FILES
        game.h
        constants.h)

# Set linker libraries for Windows
set(LINKER_LIBRARIES_WINDOWS
        user32
        gdi32
        opengl32
        gdiplus
        Shlwapi
        dwmapi
        )

# Set linker libraries for Linux
set(LINKER_LIBRARIES_LINUX
        X11
        GL
        pthread
        png
        stdc++fs)

# Add config.cfg to the build directory
configure_file(config.cfg config.cfg COPYONLY)

# Add to executable for Windows and Linux
if(WIN32)
    add_executable(GameOfLife ${SOURCE_FILES} ${HEADER_FILES})
    target_link_libraries(GameOfLife ${LINKER_LIBRARIES_WINDOWS})
elseif(UNIX)
    add_executable(GameOfLife ${SOURCE_FILES} ${HEADER_FILES})
    target_link_libraries(GameOfLife ${LINKER_LIBRARIES_LINUX})
endif()
