/*
 * Expression.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Expression.hpp"

// Required in order to use lexical cast
namespace boost{
	void throw_exception(std::exception const &e){}
}

namespace vlang {

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// INT
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string IntConstExprAST::dump() const {
	return boost::lexical_cast<std::string>(m_val);
}

std::string UnaryIntExprAST::dump() const {
	return m_operation + m_operand->dump();
}

std::string BinaryIntExprAST::dump() const {
	return m_left->dump() + m_operation + m_right->dump();
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DOUBLE
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string DoubleConstExprAST::dump() const {
	return boost::lexical_cast<std::string>(m_val);
}

std::string VariableExprAST::dump() const {
	return boost::lexical_cast<std::string>(to_str(m_type)) + " " + m_name;
};

} // ;vlang
