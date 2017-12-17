#include "generator.h"

template <class T>
template <size_t n>
static_sequence_generator::static_sequence_generator(const T (&seq)[n])
		: size(n), count(0), data((const T*) &seq) {}

template <class T>
bool static_sequence_generator::has_end() const {
	return count >= size;
}

template <class T>
T static_sequence_generator::generate() {
	if (!has_end()) return data [count++];
	throw ExceptionGeneratorEmpty ();
}

template <class T>
generator<T>* static_sequence_generator::clone() const {
	return new static_sequence_generator (data, size);
}

template <class T>
size_t static_sequence_generator::get_size() const {
	return count;
}

int random_int_generator::generate() {
	if (!has_end()) { count++; return (int) mrand48(); }
	throw ExceptionGeneratorEmpty();
}

bool random_int_generator::has_end() const {
	return count >= size;
}

generator<int>* random_int_generator::clone() const {
	return new random_int_generator(size);
}

node_name_generator::node_name_generator(const std::string &_b)
		: count(0), base(_b) {}

std::string node_name_generator::generate() {
	return base + std::to_string(count++);
}

bool node_name_generator::has_end() const { return false; }

generator<std::string>* node_name_generator::clone() const {
	throw ExceptionNodeNameGeneratorClone();
}