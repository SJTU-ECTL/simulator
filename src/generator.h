#ifndef __SIMULATOR__GENERATOR__H__
#define __SIMULATOR__GENERATOR__H__

#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>
#include <bitset>
#include "exception.h"

template <class T>
class generator {
public:
	virtual T generate() = 0;
	virtual bool has_end() const = 0;
	virtual generator<T> *clone() const = 0;
};

template <class T>
class static_sequence_generator : public generator<T> {
	const T *data;
	size_t size, count;
	static_sequence_generator(const T* _data, size_t _size)
			: data(_data), size(_size), count(0) {};
public:
	template <size_t n> explicit static_sequence_generator(const T (&seq)[n]);
	T generate();
	bool has_end() const ;
	generator<T> *clone() const ;
	size_t get_size() const ;
};

class random_int_generator : public generator<int> {
	size_t count, size;
public:
	explicit random_int_generator(size_t _s) : count(0), size(_s) {}
	int generate() override ;
	bool has_end() const override ;
	generator<int> *clone() const override ;
};

class node_name_generator : public generator<std::string> {
	std::string base;
	size_t count;
public:
	explicit node_name_generator(const std::string &_b);
	std::string generate() override ;
	bool has_end() const override ;
	generator<std::string> *clone() const override ;
};

class pattern_generator : public generator<std::vector<int>> {
public:
	virtual size_t input_num() const = 0;
};

class full_pattern_generator : public pattern_generator {
	size_t input_size;
	long long unsigned int current_pattern;
	long long unsigned int max_val;
public:
	explicit full_pattern_generator(size_t _inp_size);
	size_t input_num() const override ;
	std::vector<int> generate() override ;
	bool has_end() const override ;
	generator<std::vector<int>> *clone() const override ;
};

class infinite_random_pattern_generator : public pattern_generator {
	size_t input_size;
public:
	explicit infinite_random_pattern_generator(size_t _inp_size);
	size_t input_num() const override ;
	std::vector<int> generate() override ;
	bool has_end() const override ;
	generator<std::vector<int>> *clone() const override ;
};

class random_pattern_generator : public infinite_random_pattern_generator {
	size_t sample_num, current_sample_count;
public:
	explicit random_pattern_generator(size_t _inp_n, size_t _smp_n);
	bool has_end() const override ;
	std::vector<int> generate() override ;
};

#endif