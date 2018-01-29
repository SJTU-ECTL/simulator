#include <iostream>
#include "simulator_interface.h"

int main() {
    auto var = simulator("./data/benchmark/C17.blif",
                         "./data/benchmark/C17_del.blif",
                         100000);
    return 0;
}