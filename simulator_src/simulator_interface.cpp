#include <iostream>
#include "bnet/bnet.h"
#include "memorize.hpp"
#include "convert_blif_cpp.h"
#include "generator.h"
#include "urandom.h"
#include "circuit_simulation.h"
#include "stopwatch.hpp"
#include "simulator_interface.h"

std::map<std::string, double> simulator(const std::string &file1_loc,
                                        const std::string &file2_loc,
                                        int simu_time,
                                        const std::string &export_loc) {
    std::map<std::string, double> simu_res;
    auto test1 = new BnetNetwork(file1_loc);
    auto test2 = new BnetNetwork(file2_loc);
    circuit_simulation simu1(test1, export_loc);
    circuit_simulation simu2(test2, export_loc);
    for (const auto &i : test1->getOutputNames()) {
        bool checker = true;
        for (const auto &j : test2->getOutputNames())
            if (i == j) checker = false;
        if (checker) assert(false);
    }
    for (const auto &i : test1->getInternalNames()) {
        bool checker = true;
        for (const auto &j : test2->getInternalNames())
            if (i == j) checker = false;
        if (checker) assert(false);
    }
    StopWatch watcher;
    auto var_ctx_1 = CONTEXT_PTR(simu1.get_simulation_context());
    auto var_ctx_2 = CONTEXT_PTR(simu2.get_simulation_context());
    watcher.take("compile over");
    std::vector<int> output1;
    std::vector<int> nodes1;
    nodes1.resize(test1->getInternalNames().size());
    output1.resize(test1->getOutputNames().size());
    std::vector<int> output2;
    std::vector<int> nodes2;
    nodes2.resize(test1->getInternalNames().size());
    output2.resize(test1->getOutputNames().size());
    std::vector<int> recorder_output(test1->getOutputNames().size(), 0);
    std::vector<int> recorder_internal(test1->getInternalNames().size(), 0);
    for (int _ = 0; _ < simu_time; _++) {
        auto temp_vec = urandom::random_bit_vec(
                (int) test1->getInputNames().size());
        var_ctx_1->circuit(temp_vec.data(), output1.data(), nodes1.data());
        var_ctx_2->circuit(temp_vec.data(), output2.data(), nodes2.data());
        for (int j = 0; j < test1->getOutputNames().size(); j++)
            if (output1[j] == output2[j])
                recorder_output[j]++;
        for (int j = 0; j < test1->getInternalNames().size(); j++)
            if (nodes1[j] == nodes2[j])
                recorder_internal[j]++;
    }
    watcher.take("runtime over");
    std::cout << "\n";
    watcher.report();
    std::cout << "\n";
    int index_output = 0;
    std::cout << "output part" << std::endl;
    for (const auto &var : test1->getOutputNames()) {
        std::cout << var << ": ";
        std::cout << recorder_output[index_output] * 1.0
                     / (double) (simu_time) * 100.0 << "%" << std::endl;
        simu_res[var] = recorder_output[index_output] * 1.0
                        / (double) (simu_time) * 100.0;
        index_output++;
    }
    if (!test1->getInternalNames().empty()) {
        std::cout << "\n";
        std::cout << "internal part" << std::endl;
        index_output = 0;
        for (const auto &var : test1->getInternalNames()) {
            std::cout << var << ": ";
            std::cout << recorder_internal[index_output] * 1.0
                         / (double) (simu_time) * 100.0 << "%" << std::endl;
            simu_res[var] = recorder_internal[index_output] * 1.0
                            / (double) (simu_time) * 100.0;
            index_output++;
        }
    }
    delete test1;
    delete test2;
    return simu_res;
};
