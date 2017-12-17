set(CMAKE_CXX_STANDARD 17)

link_directories(${lib_directory})

add_executable(simu_test
		main.cpp
		simu_res_type.h simu_res_type.cpp
		exception.h)
target_link_libraries(simu_test bnet)