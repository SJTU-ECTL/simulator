#include "circuit_simulation.h"
#include "urandom.h"
#include <iostream>
#include "bnet/bnet.h"
#include <dlfcn.h>
#include "generator.h"

#define STRING_CMD_BASE "g++ -std=c++14 -shared -fPIC -Ofast -march=native \\\n"

circuit_simulation::circuit_simulation
        (BnetNetwork *bnet,
         const std::string &loc)
        : __b_net(bnet)
        , __converter(bnet, loc),
          context()
        , __source_location(__converter.get_loc() +
                            __converter.get_name()) {}

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

const std::string& circuit_simulation::source_location() {
    return __source_location;
}

circuit_simulation::~circuit_simulation() {
    if (!context.is_valid()) return;
    auto ctx = (SimulationContext*) context.get();
    dlclose(ctx->libHandle);
    delete ctx;
}

circuit_diff_context::circuit_diff_context(const std::string &_ckt_loc1,
                                           const std::string &_ckt_loc2,
                                           const std::string &_e_loc1,
                                           const std::string &_e_loc2)
        : ckt_loc1(_ckt_loc1), ckt_loc2(_ckt_loc2)
        , export_loc1(_e_loc1), export_loc2(_e_loc2)
        , bnet1(new BnetNetwork(ckt_loc1))
        , bnet2(new BnetNetwork(ckt_loc2))
        , simu1(bnet1, export_loc1)
        , simu2(bnet2, export_loc2) {}

bool circuit_diff_context::io_same() {
    for (auto &i : bnet1->getInputNames())
        for (auto &j : bnet2->getInputNames())
            if (i != j) return false;
    for (auto &i : bnet1->getOutputNames())
        for (auto &j : bnet2->getOutputNames())
            if (i != j) return false;
    return true;
}

double circuit_diff_context::generate_diff_error_rate(int times) {
    auto var_ctx1 = CONTEXT_PTR(simu1.get_simulation_context());
    auto var_ctx2 = CONTEXT_PTR(simu2.get_simulation_context());
    assert(io_same());
    const size_t out_size = bnet1->getOutputNames().size();
    std::vector<int> output1;
    std::vector<int> output2;
    std::vector<int> nodes1;
    std::vector<int> nodes2;
    nodes1.resize(bnet1->getInternalNames().size());
    nodes2.resize(bnet1->getInternalNames().size());
    output1.resize(bnet1->getOutputNames().size());
    output2.resize(bnet1->getOutputNames().size());
    random_pattern_generator gen(bnet1->getInputNames().size(), (size_t) times);
    int count = 0;
    while (!gen.has_end()) {
        auto gen_what = gen.generate();
        var_ctx1->circuit(gen_what.data(), output1.data(), nodes1.data());
        var_ctx2->circuit(gen_what.data(), output2.data(), nodes2.data());
        size_t __size = output1.size();
        for (int i = 0; i < __size; i++)
            if (output1[i] == output2[i])
                count++;
    }
    return (double) count * 1.0 /
           (double) (times * bnet1->getOutputNames().size());
}