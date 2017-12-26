#include "circuit_simulation.h"

circuit_simulation::circuit_simulation
		(BooleanNetwork *bnet,
		 const std::string &loc)
		: __b_net(bnet)
		, __converter(bnet, loc) {}

void* circuit_simulation::get_simulation_context() {
	if (context.is_valid()) return context.get();

	return context.get();
}