#include "cudd_bnet.h"
#include <string>

class BooleanNetwork {
private:
    BnetNetwork *net;
public:
    BooleanNetwork(const std::string &file);

    ~BooleanNetwork();

    void PrintNetwork();

	size_t input_num() const;

	size_t output_num() const;
};