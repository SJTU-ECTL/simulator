#include <iostream>
#include "bnet/bnet.h"
#include "bnet/memorize.hpp"
#include "convert_blif_cpp.h"
#include "generator.h"
#include "urandom.h"

#define __DEBUG__MODULES__

int main() {
#ifdef __DEBUG__MODULES__
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
	// BooleanNetwork *test = new BooleanNetwork("./src/benchmark/apex4.blif");
	// convert_blif_cpp cv(test, "./src/exported.cpp");
	// cv.exporter();
#endif
	return 0;
}