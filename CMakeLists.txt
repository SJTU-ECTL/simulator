cmake_minimum_required(VERSION 3.8)
project(Simulator)
set(CMAKE_CXX_STANDARD 17)

set(lib_directory ${PROJECT_SOURCE_DIR}/lib)
set(bnet_include_directory ${PROJECT_SOURCE_DIR}/include/bnet)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(boolean_network)
add_subdirectory(dummy_target)


add_subdirectory(src)
