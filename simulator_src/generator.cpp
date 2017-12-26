#include "generator.h"
#include "urandom.h"

int random_int_generator::generate() {
	if (!has_end()) { __count++; return (int) mrand48(); }
	throw ExceptionGeneratorEmpty();
}

bool random_int_generator::has_end() const {
	return __count >= __size;
}

generator<int>* random_int_generator::clone() const {
	return new random_int_generator(__size);
}

node_name_generator::node_name_generator(const std::string &_b)
		: __count(0), __base(_b) {}

std::string node_name_generator::generate() {
	return __base + std::to_string(__count++);
}

bool node_name_generator::has_end() const { return false; }

generator<std::string>* node_name_generator::clone() const {
	throw ExceptionNodeNameGeneratorClone();
}

full_pattern_generator::full_pattern_generator(size_t _inp_size)
		: input_size(_inp_size), current_pattern(0u),
		  max_val(((long long unsigned int) 1u) << _inp_size) {
	if (input_size <= 0) throw ExceptionFPGInputLeqZero();
	if (input_size > sizeof(long long unsigned int) * 8 - 1)
		throw ExceptionFPGInputTooLarge();
}

size_t full_pattern_generator::input_num() const {
	return this->input_size;
}

std::vector<int> full_pattern_generator::generate() {
	if (this->has_end()) throw ExceptionGeneratorEmpty();
	std::vector<int> res;
	for (int i = 0; i < input_size; i++) {
		if ((current_pattern & ((long long unsigned int) 1u) << i) > 0)
			res.push_back(1);
		else res.push_back(0);
	}
	current_pattern++;
	return res;
}

bool full_pattern_generator::has_end() const {
	return current_pattern == max_val;
}

generator<std::vector<int>>* full_pattern_generator::clone() const {
	return new full_pattern_generator(input_size);
}

infinite_random_pattern_generator::infinite_random_pattern_generator
		(size_t _inp_size) : input_size(_inp_size) {}

bool infinite_random_pattern_generator::has_end() const { return false; }

std::vector<int> infinite_random_pattern_generator::generate() {
	return urandom::random_bit_vec((int) input_size);
}

generator<std::vector<int>>* infinite_random_pattern_generator::clone() const {
	assert(false);
	return nullptr;
}

size_t infinite_random_pattern_generator::input_num() const {
	return input_size;
}

random_pattern_generator::random_pattern_generator(size_t _inp_n, size_t _smp_n)
		: infinite_random_pattern_generator(_inp_n),
		  sample_num(_smp_n), current_sample_count(0) {}

bool random_pattern_generator::has_end() const {
	return current_sample_count == sample_num;
}

std::vector<int> random_pattern_generator::generate() {
	assert(!this->has_end());
	auto _ = infinite_random_pattern_generator::generate();
	current_sample_count++;
	return _;
}