#ifndef __BITSET__MANIPULATE__HPP__
#define __BITSET__MANIPULATE__HPP__

#include <boost/functional/hash.hpp>
#include <boost/dynamic_bitset.hpp>
#include <string>

typedef boost::dynamic_bitset<> dyn_bit_set;

static inline std::string bit_set_2_str(const dyn_bit_set &d) {
	std::string s;
	boost::to_string(d, s);
	return s;
}

static inline dyn_bit_set ones(size_t length, size_t n) {
	/* fill length-bitset with n 1 in least significant */
	dyn_bit_set rua(n, 0);
	rua.set().resize(length);
	return rua;
}

static inline dyn_bit_set fill_with(size_t n, dyn_bit_set f) {
	/* copy first n bits, fill total with the n bits */
	f &= ones(f.size(), n);
	while (f.size() > n) {
		f |= f << n;
		n <<= 1;
	}
	return f;
}

static inline dyn_bit_set extract(const dyn_bit_set &bs,
								  const std::vector<size_t> &vc) {
	dyn_bit_set res(vc.size());
	for (int i = 0; i < vc.size(); i++)
		res[i] = bs.test(vc[i]);
	return res;
}

static inline dyn_bit_set extract(const dyn_bit_set &bs,
								  const dyn_bit_set &ibs) {
	std::vector<size_t> temp;
	size_t loc = ibs.find_first();
	while (loc != ibs.npos) {
		temp.push_back(loc);
		loc = ibs.find_next(loc);
	}
	return extract(bs, temp);
}

static inline size_t get_lsb(size_t bs) {
	assert(bs != 0);
	return __builtin_ctz(bs);
}

static inline size_t get_lsb(const dyn_bit_set &bs) {
	assert(!bs.empty());
	return bs.find_first();
}

static inline dyn_bit_set cat_bit_set(const dyn_bit_set &lhs,
									  const dyn_bit_set &rhs) {
	dyn_bit_set res = rhs;
	dyn_bit_set temp = rhs;
	temp.resize(lhs.size() + rhs.size());
	res.resize(lhs.size() + rhs.size());
	res |= temp << (rhs.size());
	return res;
}

static inline dyn_bit_set operator+(const dyn_bit_set &lhs,
									const dyn_bit_set &rhs) {
	return cat_bit_set(lhs, rhs);
}

/* not understanding what this function is for */
static std::vector<dyn_bit_set> breakdown(const dyn_bit_set &bs,
										  const dyn_bit_set &row,
										  const dyn_bit_set &col) {
	assert(row.size() == col.size()
		   && row == ~col
		   && bs.size() == (1ul << row.size()));
	size_t width = 1ul << (row.count());
	size_t height = 1ul << (col.count());
	size_t total = row.size();
	std::vector<dyn_bit_set> result(height, dyn_bit_set(width));
	for (size_t i = 0; i < bs.size(); i++) {
		auto curr_int = dyn_bit_set(total, i);
		auto key = extract(curr_int, col);
		auto row_key = extract(curr_int, row);
		result[key.to_ulong()][row_key.to_ulong()] = bs[i];
	}
	return result;
}

template <typename T>
std::vector<T> pick_by_dbs(const std::vector<T> &vc,
						   const dyn_bit_set &dbs) {
	assert(vc.size() == dbs.size());
	std::vector<T> res;
	for (size_t i = 0; i < dbs.size(); i++)
		if (dbs.test(i))
			res.push_back(vc.at(i));
	return res;
}

template <typename T>
dyn_bit_set vec_2_dbs(const std::vector<T> &vc) {
	dyn_bit_set res(vc.size());
	auto data = vc.data();
	for (int i = 0; i < vc.size(); i++)
		res[i] = data[i] > 0;
	return res;
}

#endif
