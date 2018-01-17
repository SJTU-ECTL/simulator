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
    std::string __source_location;
public:
    explicit circuit_simulation(BnetNetwork *bnet,
                                const std::string &loc);
    void *get_simulation_context();
    const std::string &source_location();
    ~circuit_simulation();
};

class circuit_diff_context {
    std::string ckt_loc1, ckt_loc2;
    BnetNetwork *bnet1, *bnet2;
    std::string export_loc1, export_loc2;
    circuit_simulation simu1, simu2;
public:
    explicit circuit_diff_context(const std::string &_ckt_loc1,
                                  const std::string &_ckt_loc2,
                                  const std::string &_e_loc1,
                                  const std::string &_e_loc2);
    bool io_same();
    double generate_diff_error_rate(int times);
};


#endif