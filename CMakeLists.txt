cmake_minimum_required( VERSION 3.22.1 )

project( visualization_tools )

set( CMAKE_CXX_STANDARD 20 )

add_library( visualization_tools SHARED visualization_tools.cpp)

target_link_libraries( visualization_tools PUBLIC SDL2pp )

target_compile_options( visualization_tools PUBLIC -fsanitize=leak -g )

set_target_properties( visualization_tools PROPERTIES PUBLIC_HEADER visualization_tools.hpp )

install( TARGETS visualization_tools
         LIBRARY DESTINATION lib 
         PUBLIC_HEADER DESTINATION include )
