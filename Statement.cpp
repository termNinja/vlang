/*
 * Statement.cpp
 * Copyright (C) 2016 Nemanja Mićović
 *
 * Distributed under terms of the MIT license.
 */

#include "Statement.hpp"

namespace vlang {

std::string ReturnStmtAST::dump(unsigned level) const {
	std::string res = "";
	while (level > 0) {
		res += "\t";
		--level;
	}
	res += "return" + m_retVal->dump();
	return res;
}

std::string PrototypeAST::dump(unsigned level) const {
	std::string res = "";
	while (level > 0) {
		res += "\t";
		--level;
	}
	res = to_str(m_retVal) + " " + m_name + "(";
	if (! m_args.empty()) {
		unsigned i = 0;
		for (; i < m_args.size()-1; ++i)
			res += to_str(m_args[i].first) + " " + m_args[i].second + ", ";
		res += to_str(m_args[i].first) + " " + m_args[i].second;
	}
	res += ")";
	return res;
};

} // ;vlang
