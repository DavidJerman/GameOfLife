cmake_minimum_required(VERSION 3.22)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)

add_executable(GameOfLife main.cpp game.cpp game.h constants.h)

target_link_libraries(GameOfLife X11)
target_link_libraries(GameOfLife GL)
target_link_libraries(GameOfLife pthread)
target_link_libraries(GameOfLife png)
target_link_libraries(GameOfLife stdc++fs)
