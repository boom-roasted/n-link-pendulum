cmake_minimum_required(VERSION 3.20)
project(nlinkpendulum LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(pendlib)
add_subdirectory(pendui)
