set(CMAKE_CXX_STANDARD 17)

link_directories(${lib_directory})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(
        simu_test
        main.cpp exception.h memorize.hpp
        generator.cpp generator.h
        urandom.cpp urandom.h
        convert_blif_cpp.cpp convert_blif_cpp.h
        circuit_simulation.cpp circuit_simulation.h
        stopwatch.hpp)

target_link_libraries(
        simu_test
        bnet
        ${CMAKE_DL_LIBS})