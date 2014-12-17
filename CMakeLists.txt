cmake_minimum_required(VERSION 2.8)
project(geopod)

set(geopod_sources geopod.cpp)
set_source_files_properties(${geopod_sources} PROPERTIES COMPILE_FLAGS "-std=c++11 -Wall -fno-exceptions")
add_executable(geopod ${geopod_sources})
