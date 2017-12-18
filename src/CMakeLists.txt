set(CMAKE_CXX_STANDARD 17)

link_directories(${lib_directory})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(simu_test
		main.cpp exception.h
		simu_res_type.cpp simu_res_type.h
		generator.cpp generator.h
		urandom.cpp urandom.h
		)

target_link_libraries(simu_test bnet)