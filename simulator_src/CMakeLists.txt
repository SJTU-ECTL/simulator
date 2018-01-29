link_directories(${lib_directory})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(simu_source_files
        generator.cpp
        urandom.cpp
        convert_blif_cpp.cpp
        circuit_simulation.cpp
        stopwatch.hpp
        simulator_interface.cpp)

set(simu_header_files
        exception.h memorize.hpp
        generator.h urandom.cpp
        convert_blif_cpp.h
        circuit_simulation.h
        simulator_interface.h)

set(install_header simulator_interface.h)

add_library(simulator_shared SHARED ${simu_source_files} ${simu_header_files})
add_library(simulator_static STATIC ${simu_source_files} ${simu_header_files})
set_target_properties(simulator_shared PROPERTIES OUTPUT_NAME "simulator")
set_target_properties(simulator_static PROPERTIES OUTPUT_NAME "simulator")

install(TARGETS simulator_shared simulator_static
        LIBRARY DESTINATION ${lib_directory}
        ARCHIVE DESTINATION ${lib_directory})
install(FILES ${install_header}
        DESTINATION ${simu_include_directory})

link_directories(${lib_directory})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_custom_target(install_simulator
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target install
        COMMENT "Installing ${PROJECT_NAME}")

add_executable(
        simu_test
        ../simulator_src/main.cpp
        ${simu_source_files}
        ${simu_header_files}
)

target_link_libraries(
        simu_test
        bnet
        ${CMAKE_DL_LIBS})