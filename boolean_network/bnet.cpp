#include "bnet.h"

static FILE *open_file(const char *filename, const char *mode);

BooleanNetwork::BooleanNetwork(const std::string &file) {
    FILE *fp;
    fp = open_file(file.c_str(), "r");
    net = Bnet_ReadNetwork(fp, 0);
    fclose(fp);
}

BooleanNetwork::~BooleanNetwork() {
    Bnet_FreeNetwork(net);
}

void BooleanNetwork::PrintNetwork() {
    Bnet_PrintNetwork(net);
}

static FILE *open_file(const char *filename, const char *mode) {
    FILE *fp;
    if (strcmp(filename, "-") == 0) {
        return mode[0] == 'r' ? stdin : stdout;
    } else if ((fp = fopen(filename, mode)) == NULL) {
        perror(filename);
        exit(1);
    }
    return (fp);
}

size_t BooleanNetwork::input_num() const {
    return (size_t) net->ninputs;
}

size_t BooleanNetwork::output_num() const {
    return (size_t) net->noutputs;
}

size_t BooleanNetwork::get_node_num() const {
	if (node_num.is_valid()) return node_num.get();
	BnetNode *node = net->nodes;
	size_t res = 0;
	while (node != nullptr) {
		res++;
		node = node->next;
	}
	node_num.set(res);
	return node_num.get();
}

size_t BooleanNetwork::get_gate_num() const {
	if (gate_num.is_valid()) return gate_num.get();
	size_t node_count = get_node_num();
	int temp = (int) node_count - net->npis;
	gate_num.set(temp >= 0 ? (size_t) temp : 0);
	return gate_num.get();
}

const std::vector<BooleanNetwork::bnode_id> &
BooleanNetwork::get_input_node_vec() const {
	if (input_node_vec.is_valid()) return input_node_vec.get();
	std::vector<std::string> temp;
	for (int i = 0; i < net->ninputs; i++)
		temp.emplace_back(net->inputs[i]);
	input_node_vec.set(temp);
	return input_node_vec.get();
}

const std::vector<BooleanNetwork::bnode_id> &
BooleanNetwork::get_output_node_vec() const {
	if (output_node_vec.is_valid()) return output_node_vec.get();
	std::vector<std::string> temp;
	for (int i = 0; i < net->noutputs; i++)
		temp.emplace_back(net->outputs[i]);
	output_node_vec.set(temp);
	return output_node_vec.get();
}

const std::set<BooleanNetwork::bnode_id> &
BooleanNetwork::get_input_node_set() const {
	if (input_node_set.is_valid()) return input_node_set.get();
	auto temp_list = this->get_input_node_vec();
	input_node_set.set(std::set<std::string>(temp_list.begin(),
											 temp_list.end()));
	return input_node_set.get();
}

const std::set<BooleanNetwork::bnode_id> &
BooleanNetwork::get_output_node_set() const {
	if (output_node_set.is_valid()) return output_node_set.get();
	auto temp_list = this->get_output_node_vec();
	output_node_set.set(std::set<std::string>(temp_list.begin(),
											  temp_list.end()));
	return output_node_set.get();
}

const std::set<BooleanNetwork::bnode_id> &
BooleanNetwork::get_total_node_set() const {
	if (total_node_set.is_valid()) return total_node_set.get();
	std::set<std::string> temp_set;
	BnetNode *node = net->nodes;
	while (node != nullptr) {
		temp_set.insert(node->name);
		node = node->next;
	}
	total_node_set.set(temp_set);
	return total_node_set.get();
}

const std::set<BooleanNetwork::bnode_id> &
BooleanNetwork::get_internal_node_set() const {
	if (internal_node_set.is_valid()) return internal_node_set.get();
	auto i_set = this->get_input_node_set();
	auto o_set = this->get_output_node_set();
	std::set<std::string> temp_set;
	BnetNode *node = net->nodes;
	while (node != nullptr) {
		if (i_set.count(node->name) != 0
			|| o_set.count(node->name) != 0) {
			node = node->next;
			continue;
		}
		temp_set.insert(std::string(node->name));
		node = node->next;
	}
	internal_node_set.set(temp_set);
	return internal_node_set.get();
}