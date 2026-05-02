cmake_minimum_required(VERSION 3.16)
project(Snake)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    /usr/local/include
)

link_directories(/usr/local/lib)

add_executable(snake
    src/main.cpp
)

target_link_libraries(snake
    raylib
    m
    pthread
    dl
    rt
    X11
)