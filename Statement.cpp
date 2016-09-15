/*
 * Statement.cpp
 * Copyright (C) 2016 Nemanja Mićović
 *
 * Distributed under terms of the MIT license.
 */

#include "Statement.hpp"

namespace vlang {

std::string getStrWithIndent(unsigned level = 0) {
	std::string res = "";
	while (level > 0) {
		res += "\t";
		--level;
	}
	return res;
}

std::string ReturnStmtAST::dump(unsigned level) const {
	std::string res = getStrWithIndent(level);
	res += "return " + m_retVal->dump();
	return res;
}

std::string PrototypeAST::dump(unsigned level) const {
	std::string res = getStrWithIndent(level);
	res += to_str(m_retVal) + " " + m_name + "(";
	if (! m_args.empty()) {
		unsigned i = 0;
		for (; i < m_args.size()-1; ++i)
			res += to_str(m_args[i].first) + " " + m_args[i].second + ", ";
		res += to_str(m_args[i].first) + " " + m_args[i].second;
	}
	res += ")";
	return res;
};

std::string ExpressionStmtAST::dump(unsigned int level) const {
	std::string res = getStrWithIndent(level);
	res += m_expr->dump();
	return res;
}

std::string FunctionAST::dump(unsigned int level) const {
	std::string res = m_proto.dump(level);
	res += " " + m_definition->dump(level);
	return res;
}

std::string BlockStmtAST::dump(unsigned level) const {
	std::string res = getStrWithIndent(level);
	res += "{\n";
	for (auto &cmd : m_cmds) {
		res += getStrWithIndent(level+1);
		res += cmd->dump(0) + ";\n";
	}
	res += getStrWithIndent(level) + "}";
	return res;
}

std::string AssignmentStmtAST::dump(unsigned level) const {
	std::string res = getStrWithIndent(level);
	if (m_type != VLANG_TYPE::NO_VAR_DECL) res += to_str(m_type) + " ";
	res += m_varName + " = " + m_expr->dump();
	return res;
}

} // ;vlang
