cmake_minimum_required(VERSION 3.20)
project(pendlib LANGUAGES CXX VERSION 0.0.1)

set(SOURCES
    Pendulum.cpp
)

add_library(pendlib ${SOURCES})
target_compile_features(pendlib PUBLIC cxx_std_20)

# Tells other projects that include this to add to the include directories.
target_include_directories(pendlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
