#include "convert_blif_cpp.h"
#include <fstream>
#include <bitset>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <cassert>
#include <deque>
#include "bnet/bnet.h"
#include "urandom.h"

using std::string;
using std::unordered_map;
using std::set;
using std::vector;

convert_blif_cpp::convert_blif_cpp
        (BnetNetwork *b_net,
         const string &export_to)
        : __b_net(b_net)
        , __export_to(export_to) {
    int random_var = abs(urandom::random_int());
    __cpp_name = "sim_ckt_" + std::to_string(random_var) + ".cpp";
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

static string getFunctionString(const vector<string> &name_list,
                                const string &truth_table) {
    assert(truth_table.length() == name_list.size());
    auto size = (int) name_list.size();
    string func_string;
    func_string.push_back('(');
    for (int i = 0; i < size; i++) {
        func_string += '(';
        if (truth_table[i] == '1') func_string += name_list[i];
        else if (truth_table[i] == '0') {
            func_string += '!';
            func_string += name_list[i];
        } else if (truth_table[i] == '-')
            func_string += '1';
        else assert(false);
        func_string += ") & ";
    }
    func_string += "1)";
    return func_string;
}

static string getSafeName(const string &name) {
    string temp;
    if (isdigit(name[0])) temp = "_";
    else temp = " ";
    temp += name;
    std::replace(temp.begin(), temp.end(), '(', '_');
    std::replace(temp.begin(), temp.end(), ')', '_');
    std::replace(temp.begin(), temp.end(), '[', '_');
    std::replace(temp.begin(), temp.end(), ']', '_');
    std::replace(temp.begin(), temp.end(), '*', '_');
    return temp;
}

static string getFunctionString(BnetNode *node) {
    const auto &truth_table_ls = node->getTruthTable();
    const auto &alias_name_list = node->getFanIns();
    vector<string> _name_list;
    for (auto &i : alias_name_list) {
        _name_list.push_back(getSafeName(i));
    }
    string func_string;
    if (!node->isOnSet()) func_string += "(!";
    func_string.push_back('(');
    for (const auto &i : truth_table_ls) {
        func_string += getFunctionString(_name_list, i);
        func_string += " | ";
    }
    func_string += "0)";
    if (!node->isOnSet()) func_string += ")";
    return func_string;
}

const std::string& convert_blif_cpp::get_loc() const {
    return __export_to;
}

const std::string& convert_blif_cpp::get_name() const {
    return __cpp_name;
}

std::vector<BnetNodeID> TopologicalSort(const BnetNetwork *net) {
    std::vector<BnetNodeID> sortedNodes;
    std::deque<BnetNodeID> nodesQueue;
    std::map<BnetNodeID, int> inDegrees;
    for (auto node:net->getNodesList()) {
        inDegrees[node->getName()] = (int) node->getFanIns().size();
        if (node->getFanIns().empty())
            nodesQueue.emplace_back(node->getName());
    }
    while (!nodesQueue.empty()) {
        BnetNodeID id = nodesQueue.front();
        sortedNodes.emplace_back(id);
        nodesQueue.pop_front();
        const BnetNode *node = net->getNodebyName(id);
        for (const auto &output:node->getFanOuts()) {
            inDegrees[output] -= 1;
            if (inDegrees[output] == 0)
                nodesQueue.push_back(output);
        }
    }
    return sortedNodes;
}

void convert_blif_cpp::exporter() {
    auto topSort = TopologicalSort(this->__b_net);
    auto inputs = __b_net->getInputNames();
    auto outputs = __b_net->getOutputNames();
    auto internals = __b_net->getInternalNames();
    size_t nInputs = inputs.size();
    size_t nOutputs = outputs.size();
    size_t nInternals = internals.size();
    std::ofstream ofile(__export_to + __cpp_name);
    if (!ofile) {
        std::cerr << "Cannot open " << __export_to + __cpp_name << std::endl;
        return;
    }
    auto inputAlias = create_alias(inputs, "input");
    auto outputAlias = create_alias(outputs, "output");
    auto internalAlias = create_alias(internals, "node");
    ofile << "// cpp built by tool\n";
    ofile << "#include <vector>\n";
    ofile << "#include <string>\n";
    ofile << "#define TYPE int\n";
    ofile << "extern \"C\" {\n"
          << "void circuit(const TYPE input[], TYPE output[], TYPE node[]);\n"
          << "std::vector<std::string> inputNode();\n"
          << "std::vector<std::string> outputNode();\n"
          << "std::vector<std::string> internalNode();\n"
          << "}\n";
    ofile << "void circuit(const TYPE input[], TYPE output[], TYPE node[]) {\n";
    for (const auto& nodeName : inputs) {
        const auto& aliasName = inputAlias.at(nodeName);
        ofile << "\tconst TYPE& " << getSafeName(nodeName) << " = " << (aliasName) << ";\n";
    }
    for (const auto& nodeName : outputs) {
        const auto& aliasName = outputAlias.at(nodeName);
        ofile << "\tTYPE& " << getSafeName(nodeName) << " = " << (aliasName) << ";\n";
    }
    for (const auto& nodeName : internals) {
        const auto& aliasName = internalAlias.at(nodeName);
        ofile << "\tTYPE& " << getSafeName(nodeName) << " = " << (aliasName) << ";\n";
    }
	for (const auto& node : topSort) {
		BnetNode* n = __b_net->getNodebyName(node);
		/* maybe reconstruct is better */
		if (n->isInput()) {}
		else if (n->getFanIns().empty()) {
			if (!n->isOnSet()) ofile << "\t" <<  getSafeName(node) << " = " << "false;\n";
			else ofile << "\t" << getSafeName(node) << " = " << "true;\n";
		} else ofile << "\t" << getSafeName(node) << " = " << getFunctionString(n) << ";\n";
	}
	// Normalize the result.
	ofile << "\tfor (int i = 0; i < " << nOutputs << "; ++i)\n";
	ofile << "\t\toutput[i] &= 0x01;\n";
	ofile << "\tfor (int i = 0; i < " << nInternals << "; ++i)\n";
	ofile << "\t\tnode[i] &= 0x01;\n";
	ofile << "}\n";
	ofile << "std::vector<std::string> inputNode() {\n";
	ofile << "return std::vector<std::string> {\n";
	int counter = 0;
	for (const string& name: inputs) {
		ofile << "\"" << getSafeName(name) << "\", ";
		if (counter % 5 == 4) ofile << "\n";
		counter ++;
	}
	ofile << "\n};\n";
	ofile << "}\n";

	ofile << "std::vector<std::string> outputNode() {\n";
	ofile << "return std::vector<std::string> {\n";
	counter = 0;
	for (const string& name: outputs) {
		ofile << "\"" << getSafeName(name) << "\", ";
		if (counter % 5 == 4) ofile << "\n";
		counter ++;
	}
	ofile << "\n};\n";
	ofile << "}\n";

	ofile << "std::vector<std::string> internalNode() {\n";
	ofile << "return std::vector<std::string> {\n";
	counter = 0;
	for (const string& name: internals) {
		ofile << "\"" << getSafeName(name) << "\", ";
		if (counter % 5 == 4) ofile << "\n";
		counter ++;
	}
	ofile << "\n};\n";
	ofile << "}\n";

	ofile.close();
}