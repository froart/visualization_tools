cmake_minimum_required( VERSION 3.22.1 )

project( VT )

set( CMAKE_CXX_STANDARD 20 )

add_library( VT SHARED vt_framebuffer.cpp )

target_link_libraries( VT PUBLIC SDL2pp )

target_compile_options( VT PUBLIC -fsanitize=leak -g )

set_target_properties( VT PROPERTIES PUBLIC_HEADER vt_framebuffer.hpp )

install( TARGETS VT
         LIBRARY DESTINATION lib 
         PUBLIC_HEADER DESTINATION include )
