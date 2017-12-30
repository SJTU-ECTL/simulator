set(CMAKE_CXX_STANDARD 17)

set(simulator_source_files
		generator.cpp urandom.cpp simu_res_type.cpp
		convert_blif_cpp.cpp circuit_simulation.cpp)

set(simulator_header_file
		exception.h stopwatch.hpp memorize.hpp
		generator.h urandom.h simu_res_type.h
		convert_blif_cpp.h circuit_simulation.h)

link_directories(${lib_directory})
include_directories(${PROJECT_SOURCE_DIR}/include)

# add_library(bnet_shared SHARED ${bnet_source_files})
# add_library(bnet_static STATIC ${bnet_source_files})

add_executable(
		simu_test
		main.cpp exception.h
		stopwatch.hpp memorize.hpp
		generator.cpp generator.h
		urandom.cpp urandom.h
		#simu_res_type.cpp simu_res_type.h
		convert_blif_cpp.cpp convert_blif_cpp.h
		#circuit_simulation.cpp circuit_simulation.h
)

target_link_libraries(
		simu_test
		bnet
		${CMAKE_DL_LIBS}
)