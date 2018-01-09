#ifndef __SIMULATOR__URANDOM__H__
#define __SIMULATOR__URANDOM__H__

#include <bitset>
#include <vector>
#include <cstddef>

class urandom {
public:
    static int random_int();
    static std::vector<int> random_int_vec(int _size);
    static std::vector<int> random_bit_vec(int _size);
    template <size_t l> static std::bitset<l> random_bit_set();
};


#endif