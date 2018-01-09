#ifndef __SIMULATOR__CIRCUIT__SIMULATION__H__
#define __SIMULATOR__CIRCUIT__SIMULATION__H__

#include "convert_blif_cpp.h"
#include "memorize.hpp"
#include <string>
#include <vector>

class BnetNetwork;

#define CONTEXT_PTR(x) (static_cast<SimulationContext*>(x))

typedef void (*CircuitFun)(const int input[],
						   int output[],
						   int node[]);

typedef std::vector<std::string> (*ConstVectorFun)();

struct SimulationContext {
	void* libHandle;
	ConstVectorFun inputNodeName;
	ConstVectorFun outputNodeName;
	ConstVectorFun internalNodeName;
	CircuitFun circuit;
};

enum NODE_TYPE {INPUT, OUTPUT, INTERNAL};

class circuit_simulation {
	convert_blif_cpp __converter;
	BnetNetwork *__b_net;
	memorize<void *> context;
public:
	explicit circuit_simulation(BnetNetwork *bnet,
								const std::string &loc);
	void *get_simulation_context();
	~circuit_simulation();
};

#endif