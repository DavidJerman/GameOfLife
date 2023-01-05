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

# Set linker libraries
set(LINKER_LIBRARIES
        X11
        GL
        pthread
        png
        stdc++fs)

# Add config.cfg to the build directory
configure_file(config.cfg config.cfg COPYONLY)
# Add to executable
add_executable(GameOfLife ${SOURCE_FILES} ${HEADER_FILES})
# Link libraries
target_link_libraries(GameOfLife ${LINKER_LIBRARIES})
