#ifndef __SIMULATOR__INTERFACE__H__
#define __SIMULATOR__INTERFACE__H__

#include <map>
#include <string>

std::map<std::string, double> simulator(const std::string &file1_loc,
                                        const std::string &file2_loc,
                                        int simu_time,
                                        const std::string &export_loc = "./");
#endif
