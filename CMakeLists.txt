cmake_minimum_required(VERSION 3.0)
project(cwd)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cwd main.cc)

target_link_libraries(cwd User32.lib)
