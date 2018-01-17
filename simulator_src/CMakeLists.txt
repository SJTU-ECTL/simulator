set(CMAKE_CXX_STANDARD 17)

link_directories(${lib_directory})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(simu_source_files
        main.cpp
        generator.cpp
        urandom.cpp
        convert_blif_cpp.cpp
        circuit_simulation.cpp
        stopwatch.hpp)

set(simu_header_files
        exception.h memorize.hpp
        generator.h urandom.cpp
        convert_blif_cpp.h
        circuit_simulation.h)

add_executable(
        simu_test
        ${simu_source_files}
        ${simu_header_files}
)

target_link_libraries(
        simu_test
        bnet
        ${CMAKE_DL_LIBS})