#include "convert_blif_cpp.h"
#include <fstream>
#include <bitset>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <cassert>
#include "bnet/bnet.h"

using std::string;
using std::unordered_map;
using std::set;
using std::vector;

convert_blif_cpp::convert_blif_cpp
		(BooleanNetwork *b_net,
		 const string &export_to)
		: __b_net(b_net)
		, __export_to(export_to) {}

void convert_blif_cpp::set(const std::string &var) {
	__export_to = var;
}

void convert_blif_cpp::set(BooleanNetwork *bn) {
	__b_net = bn;
}

void convert_blif_cpp::set(BooleanNetwork *bn,
						   const std::string &str) {
	__b_net = bn;
	__export_to = str;
}

/**
 * @brief: string functions convert strings to c/cpp lines
 * @return: a line of c/cpp code representing the
 * */

static string notG(const string& i) {
	return "(" + ("~" + i) + ")";
}

static string andG(const string& i1,
				   const string& i2) {
	return "(" + i1 + " & " + i2 + ")";
}

static string xorG(const string& i1,
				   const string& i2) {
	return "(" + i1 + " ^ " + i2 + ")";
}

static unordered_map<string, string>
create_alias(const set<string>& set,
			 const string& setName) {
	int counter = 0;
	unordered_map<string, string> alias;
	alias.clear();
	for (auto &i : set) {
		assert(alias.count(i) == 0);
		alias[i] = setName + "[" + std::to_string(counter) + "]";
		counter++;
	}
}

static unordered_map<string, string>
create_alias(const vector<string>& set,
			 const string& setName) {
	int counter = 0;
	unordered_map<string, string> alias;
	alias.clear();
	for (auto &i : set) {
		assert(alias.count(i) == 0);
		alias[i] = setName + "[" + std::to_string(counter) + "]";
		counter++;
	}
	return alias;
}

static string get_func_string(const string& i1,
							  const string& i2,
							  BnetNode* node) {
	std::bitset<4> truthTable(0);
	BnetTabline* f = node->f;
	assert(f != nullptr);
	while (f != nullptr) {
		string line = f->values;
		assert(line.size() == 2);
		if (line == "00")
			truthTable[0] = true;
		else if (line == "01")
			truthTable[1] = true;
		else if (line == "10")
			truthTable[2] = true;
		else if (line == "11")
			truthTable[3] = true;
		else if (line == "-0")
			truthTable[0] = truthTable [2] = true;
		else if (line == "0-")
			truthTable[0] = truthTable [1] = true;
		else if (line == "-1")
			truthTable[1] = truthTable [3] = true;
		else if (line == "1-")
			truthTable[2] = truthTable [3] = true;
		else if (line == "--") {
			truthTable[0] = truthTable[1] = true;
			truthTable[2] = truthTable[3] = true;
		} else {
			std::cerr << "line = " << line << std::endl;
			assert(false);
		}
		f = f->next;
	}
	if (node->polarity == 1) truthTable.flip();
	switch (truthTable.to_ulong()) {
		case 0x0: return "false";
		case 0xF: return "true";
		case 0x1: return andG(notG(i1), notG(i2));
		case 0x2: return andG(notG(i1), i2);
		case 0x3: return notG(i1);
		case 0x4: return andG(i1, notG(i2));
		case 0x5: return notG(i2);
		case 0x6: return xorG(i1, i2);
		case 0x7: return notG(andG(i1, i2));
		case 0x8: return andG(i1, i2);
		case 0x9: return notG(xorG(i1, i2));
		case 0xA: return i2;
		case 0xB: return notG(andG(i1, notG(i2)));
		case 0xC: return i1;
		case 0xD: return notG(andG(notG(i1) , i2));
		case 0xE: return notG(andG(notG(i1), notG(i2)));
		default:
			assert(false);
	}
}

static string getFunctionString(const string& i,
								BnetNode* node) {
	std::bitset<2> truthTable(0);
	BnetTabline* f = node->f;
	assert(f != nullptr);
	while (f != nullptr) {
		string line = f->values;
		assert(line.size() == 1);
		if (line == "0")
			truthTable[0] = true;
		else if (line == "1")
			truthTable[1] = true;
		else if (line == "-")
			truthTable[0] = truthTable [1] = true;
		else {
			std::cerr << "line = " << line << std::endl;
			assert(false);
		}
		f = f->next;
	}
	if (node->polarity == 1) truthTable.flip();
	switch (truthTable.to_ulong()) {
		case 0x0: return "false";
		case 0x3: return "true";
		case 0x1: return notG(i);
		case 0x2: return i;
		default:
			assert(false);
	}
}

/**
 * @brief: function export the bnet into a cpp file
 * */

void convert_blif_cpp::exporter() {

}