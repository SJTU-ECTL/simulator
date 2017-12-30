#include <iostream>
#include "bnet/bnet.h"

#define __DEBUG__MODULES__

int main() {
#ifndef __DEBUG__MODULES__
	int _var[10]; for (int _ = 0; _ < 10; _++) _var[_] = _;
	static_sequence_generator<int> sa(_var);
	while (!sa.has_end()) std::cout << sa.generate() << std::endl;
	std::cout << "Static sequence generator check over" << std::endl;
	memorize<int> var_int_m;
	var_int_m.set(100);
	std::cout << var_int_m.get() << std::endl;
	std::cout << "Memorize class check over" << std::endl;
	for (int i = 0; i < 10; i++) {
		auto rb = urandom::random_bit_vec(10);
		for (const auto &_i : rb)
			std::cout << _i << " ";
		std::cout << std::endl;
	}
	std::cout << "urandom check over" << std::endl;
	BooleanNetwork *test = new BooleanNetwork("./simulator_src/benchmark/C880.blif");
	circuit_simulation simu(test, "./simulator_src/");
	size_t inp_num = test->input_num();
	auto temp_vec = urandom::random_bit_vec(inp_num);

	StopWatch watcher;
	auto var_ctx = CONTEXT_PTR(simu.get_simulation_context());
	watcher.take("compile over");

	std::vector<int> output;
	std::vector<int> nodes;
	nodes.resize(test->get_internal_node_set().size());
	output.resize(test->output_num());

	for (int i = 0; i < 100000; i++) {
		var_ctx->circuit(temp_vec.data(), output.data(), nodes.data());
	}
	watcher.take("runtime over");
	watcher.report();
#endif
	return 0;
}