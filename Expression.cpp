/*
 * Expression.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Expression.hpp"

namespace vlang {

std::string IntConstExprAST::dump() const {
	return boost::lexical_cast<std::string>(m_val);
}

std::string DoubleConstExprAST::dump() const {
	return boost::lexical_cast<std::string>(m_val);
}

} // ;vlang
