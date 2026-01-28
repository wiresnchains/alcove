cmake_minimum_required(VERSION 3.20)

project(alcove LANGUAGES CXX)

add_executable(alcove "src/main.cpp")

set_property(TARGET alcove PROPERTY CXX_STANDARD 20)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(alcove PRIVATE fmt::fmt)
