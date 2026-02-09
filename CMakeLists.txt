cmake_minimum_required(VERSION 3.20)

project(alcove LANGUAGES CXX)

add_executable(alcove "src/main.cpp"
    "src/alcove.cpp"
    "src/alcove_util.cpp")

target_compile_definitions(alcove PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:_RELEASE>
)

set_property(TARGET alcove PROPERTY CXX_STANDARD 20)

find_package(fmt CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)

target_link_libraries(alcove PRIVATE fmt::fmt)
target_link_libraries(alcove PRIVATE cxxopts::cxxopts)
