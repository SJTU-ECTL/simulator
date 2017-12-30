#ifndef __SIMULATOR__CONVERT__BLIF__CPP__H__
#define __SIMULATOR__CONVERT__BLIF__CPP__H__

#include <string>

class BnetNetwork;

/**
 * @brief a converter class for BooleanNetwork to cpp
 * @details a BooleanNetwork and a export location
 * 			two member functions:
 * 			constructor with network and export location
 * 			`exporter` can export it to `export location`
 */

class convert_blif_cpp {
	BnetNetwork *__b_net;
	std::string __export_to;
	std::string __cpp_name;
public:
	explicit convert_blif_cpp(BnetNetwork *,
							  const std::string &);
	void exporter();
	const std::string &get_loc() const;
	const std::string &get_name() const;
	~convert_blif_cpp() = default;

};


#endif