#include "simu_res_type.h"
#include <algorithm>

simulation_result::simulation_result
		(BooleanNetwork *_net,
		 size_t _s) : sample_num(_s) {
	size_t net_inpNum = _net->input_num();
	size_t net_outNum = _net->output_num();

	for (auto &i : {std::make_pair(this->input_result_vec, net_inpNum),
					std::make_pair(this->output_result_vec, net_outNum)}) {
		i.first.reserve(i.second);
		for (auto &_ : i.first) _.reserve(i.second);
	}

}