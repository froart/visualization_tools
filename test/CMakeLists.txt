cmake_minimum_required( VERSION 3.22.1 )

project( test VERSION 1.0 LANGUAGES CXX )

add_executable( test test.cpp )

set( CMAKE_CXX_STANDARD 20 )

find_package( SDL2pp REQUIRED )
target_link_libraries( test PUBLIC VT SDL2pp::SDL2pp )

target_compile_options( test PUBLIC -fsanitize=leak -g )


