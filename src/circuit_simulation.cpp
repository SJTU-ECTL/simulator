#include "circuit_simulation.h"

circuit_simulation::circuit_simulation
		(BooleanNetwork *bnet,
		 const std::string &loc)
		: __b_net(bnet)
		, __converter(bnet, loc) {}

void* circuit_simulation::get_simulation_context() {
	if (context.is_valid()) return context.get();
	// std::string cmd = "g++ -std=c++14 -shared -fPIC -Ofast -march=native " + source + " -o " + library;
	return context.get();
}