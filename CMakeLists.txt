cmake_minimum_required( VERSION 3.22.1 )

project( VT VERSION 1.0 LANGUAGES CXX )

file( GLOB LIBRARY_SOURCES "./src/*.cpp" )
file( GLOB LIBRARY_HEADERS "./include/*.hpp" )

add_library( VT SHARED ${LIBRARY_SOURCES} )

set( CMAKE_CXX_STANDARD 20 )
target_compile_options( VT PUBLIC -fsanitize=leak -g )
find_package( SDL2pp REQUIRED )
target_link_libraries( VT PUBLIC SDL2 SDL2pp::SDL2pp )
# target_include_directories( VT PUBLIC /usr/include/SDL2 )

install( TARGETS VT
         LIBRARY DESTINATION /usr/lib/ )

install( FILES
         ${LIBRARY_HEADERS}
         DESTINATION /usr/include/ )
