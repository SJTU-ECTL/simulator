#include <iostream>
#include "bnet/bnet.h"
#include "generator.h"

int main() {
	BooleanNetwork *a;
	std::cout << "Hello world!" << std::endl;
	int _var[100]; for (int _ = 0; _ < 100; _++) _var[_] = _;
	static_sequence_generator<int> sa(_var);
	while (!sa.has_end()) {
		std::cout << sa.generate() << std::endl;
	}
	return 0;
}