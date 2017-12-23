#ifndef __SIMULATOR__CONVERT__BLIF__CPP__H__
#define __SIMULATOR__CONVERT__BLIF__CPP__H__

#include <string>

class BooleanNetwork;

/**
 * @brief a converter class for BooleanNetwork to cpp
 * @details a BooleanNetwork and a export location
 * 			two member functions:
 * 			constructor with network and export location
 * 			`export` can export it to `export location`
 */

class convert_blif_cpp {
	BooleanNetwork *__b_net;
	std::string __export_to;
public:
	explicit convert_blif_cpp(BooleanNetwork *,
							  const std::string &);
	void set(BooleanNetwork *);
	void set(const std::string &);
	void set(BooleanNetwork *,
			 const std::string &);
	void export();

	~convert_blif_cpp() = default;
};


#endif