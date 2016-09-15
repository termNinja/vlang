/*
 * Expression.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Expression.hpp"

// Required in order to use lexical cast
namespace boost{
	void throw_exception(std::exception const &e){(void)(e);}
}

namespace vlang {

std::string ConstIntExprAST::dump(unsigned level) const {
	std::string res = "";
	while (level > 0) res += "\t";
	res += boost::lexical_cast<std::string>(m_val);
	return res;
}

std::string ConstDoubleExprAST::dump(unsigned level) const {
	std::string res = "";
	while (level > 0) res += "\t";
	res += boost::lexical_cast<std::string>(m_val);
	return res;
}


std::string UnaryExprAST::dump(unsigned level) const {
	(void)level;
	return m_operation + m_operand->dump(0);
}

std::string BinaryExprAST::dump(unsigned level) const {
	(void)level;
	return m_left->dump(0) + m_operation + m_right->dump(0);
}

std::string BlockExprAST::dump(unsigned level) const {
	std::string res = "";
	unsigned tmplvl = level;
	while (tmplvl > 0) res += "\t";
	++level;
	res += "{\n";
	for (auto &cmd : m_cmds) {
		tmplvl = level;
		while (tmplvl > 0) res += "\t";
		res += cmd->dump(0);
	}
	return res;
}

std::string VariableExprAST::dump(unsigned level) const {
	(void)level;
	return boost::lexical_cast<std::string>(to_str(m_type)) + " " + m_name;
};

} // ;vlang
