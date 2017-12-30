#include "simu_res_type.h"

simulation_result::simulation_result
		(BnetNetwork *_net,
		 size_t _s) : sample_num(_s) {
	size_t net_inpNum = _net->input_num();
	size_t net_outNum = _net->output_num();
	size_t net_intNum = _net->get_internal_node_set().size();
	this->input_result_vec.resize(net_inpNum);
	this->output_result_vec.resize(net_outNum);
	this->internal_result_vec.resize(net_intNum);
	for (auto &_ : this->input_result_vec) _.resize(net_inpNum);
	for (auto &_ : this->output_result_vec) _.resize(net_outNum);
	for (auto &_ : this->internal_result_vec) _.resize(net_intNum);
}

