#include "cudd_bnet.h"
#include "memorize.hpp"
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>

class BooleanNetwork {

public:
	typedef std::string bnode_id;

	struct fanout_free_corn {
		bnode_id __name;
		std::set<bnode_id> __input_set;
		std::set<bnode_id> __node_set;
		std::set<bnode_id> __total_set;
	};

private:
	BnetNetwork *net;
	mutable memorize<size_t>                               node_num;
	mutable memorize<size_t>                               gate_num;
	mutable memorize<std::vector<bnode_id>>                input_node_vec;
	mutable memorize<std::set   <bnode_id>>                input_node_set;
	mutable memorize<std::vector<bnode_id>>                output_node_vec;
	mutable memorize<std::set   <bnode_id>>                output_node_set;
	mutable memorize<std::set   <bnode_id>>                total_node_set;
	mutable memorize<std::set   <bnode_id>>                internal_node_set;
	mutable memorize<std::vector<bnode_id>>                topo_sort_node_vec;

public:
	explicit BooleanNetwork(const std::string &file);

	~BooleanNetwork();

	void PrintNetwork();

	size_t input_num() const;
	size_t output_num() const;
	size_t get_node_num() const;
	size_t get_gate_num() const;

	const std::set<bnode_id>    &get_input_node_set() const ;
	const std::vector<bnode_id> &get_input_node_vec() const ;
	const std::set<bnode_id>    &get_output_node_set() const ;
	const std::vector<bnode_id> &get_output_node_vec() const ;
	const std::set<bnode_id>    &get_internal_node_set() const ;
	const std::set<bnode_id>    &get_total_node_set() const ;

	BnetNode *getNodesList() const ;
	BnetNode *getNodebyName(const std::string &name) const ;
	const std::vector<bnode_id>& topologicalSort() const ;
};