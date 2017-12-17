#ifndef __SIMULATION__RES__TYPE__H__
#define __SIMULATION__RES__TYPE__H__

#include <vector>
#include <cstdlib>
#include <string>
#include "bnet/bnet.h"

typedef struct compare_result {
	size_t error_number;
	size_t sample_number;
} compare_result_t;

typedef struct simulation_result {

	size_t sample_num;

	std::vector<std::vector<int>> input_result_vec;
	std::vector<std::vector<int>> output_result_vec;
	std::vector<std::vector<int>> internal_result_vec;

	std::vector<std::string> input_name_vec;
	std::vector<std::string> output_name_vec;
	std::vector<std::string> internal_name_vec;

	explicit simulation_result(BooleanNetwork *_net,
							   size_t _sample_num);

} simulation_result_t;


#endif