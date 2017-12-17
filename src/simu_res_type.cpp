/*
#include "simu_res_type.h"

simulation_result::simulation_result
		(BooleanNetwork *b_net,
		 size_t sample_num) : sample_num(sample_num) {
	size_t input_num = b_net->input_num();
	size_t output_num = b_net->output_num();
	this->input_result.resize(input_num);
	this->output_result.resize(output_num);
	for (auto &v : input_result) v.resize(input_num);
	for (auto &v : output_result) v.resize(output_num);
}*/