#include "urandom.h"
#include <random>

static std::mt19937 engine(std::random_device{}());

int count = 0;

int urandom::random_int() {
	return (int) engine();
}

std::vector<int> urandom::random_int_vec(int _size) {
	std::vector<int> v;
	for (int _ = 0; _ < _size; _++) v.push_back((int) engine());
	return v;
}

std::vector<int> urandom::random_bit_vec(int _size) {
	std::bernoulli_distribution d;
	std::vector<int> v;
	for (int i = 0; i < _size; ++i)
		v.push_back(d(engine));
	return v;
}

template <size_t l>
std::bitset<l> urandom::random_bit_set() {
	std::bernoulli_distribution d;
	std::bitset<l> v;
	for (int i = 0; i < v.size(); ++i)
		v[i] = d(engine);
	return v;
}