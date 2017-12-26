#include "circuit_simulation.h"
#include "urandom.h"
#include <iostream>
#include "bnet/bnet.h"
#include <dlfcn.h>

#define STRING_CMD_BASE "g++ -std=c++14 -shared -fPIC -Ofast -march=native \\\n"

circuit_simulation::circuit_simulation
		(BooleanNetwork *bnet,
		 const std::string &loc)
		: __b_net(bnet)
		, __converter(bnet, loc),
		  context() {}

void* circuit_simulation::get_simulation_context() {
	if (context.is_valid()) return context.get();
	__converter.exporter();
	auto source_loc = __converter.get_loc();
	auto source_name = __converter.get_name();
	std::string lib_name = source_loc + source_name + ".so";
	std::string cmd = STRING_CMD_BASE
					  + source_loc + source_name + " -o \\\n"
					  + lib_name;
	std::cout << "Executing command:\n" << cmd << std::endl;
	int ret_msg = system(cmd.c_str());
	assert(ret_msg == 0);
	std::cout << "Loading library:\n" << lib_name << std::endl;
	void *lib_handle = dlopen(lib_name.c_str(), RTLD_NOW | RTLD_LOCAL);
	if (lib_handle == nullptr) {
		std::cerr << dlerror() << std::endl;
		assert(lib_handle != nullptr);
	}
	// CircuitFun
	auto circuit = (CircuitFun)dlsym(lib_handle, "circuit");
	assert(circuit != nullptr);
	// ConstVectorFun
	auto inputNode = (ConstVectorFun)dlsym(lib_handle, "inputNode");
	assert(inputNode != nullptr);
	auto outputNode = (ConstVectorFun)dlsym(lib_handle, "outputNode");
	assert(outputNode != nullptr);
	auto internalNode = (ConstVectorFun)dlsym(lib_handle, "internalNode");
	assert(internalNode != nullptr);
	auto ctx = new SimulationContext;
	ctx->libHandle = lib_handle;
	ctx->inputNodeName = inputNode;
	ctx->outputNodeName = outputNode;
	ctx->internalNodeName = internalNode;
	ctx->circuit = circuit;
	context.set(ctx);
	return context.get();
}

circuit_simulation::~circuit_simulation() {
	if (!context.is_valid()) return;
	auto ctx = (SimulationContext*) context.get();
	dlclose(ctx->libHandle);
	delete ctx;
}