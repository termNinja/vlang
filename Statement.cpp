/*
 * Statement.cpp
 * Copyright (C) 2016 Nemanja Mićović
 *
 * Distributed under terms of the MIT license.
 */

#include "Statement.hpp"

namespace vlang {

std::string indent_style = "    ";

std::string getStrWithIndent(int level = 0) {
    if (level < 0) return "";
    std::string res = "";
    while (level > 0) {
        res += indent_style;
        --level;
    }
    return res;
}

std::string ReturnStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    res += "return " + m_retVal->dump() + ";";
    return res;
}

std::string PrototypeAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    res += to_str(m_retVal) + " " + m_name + "(";
    if (! m_args.empty()) {
        unsigned i = 0;
        for (; i < m_args.size()-1; ++i)
            res += to_str(m_args[i].first) + " " + m_args[i].second + ", ";
        res += to_str(m_args[i].first) + " " + m_args[i].second;
    }
    res += ");";
    return res;
};

std::string FunctionAST::dump(int level) const {
    std::string res = m_proto.dump(level);
    res.erase(res.size()-1, res.size());        // remove ';' from the end of proto
    res += " " + m_definition->dump(level+1);
    return res;
}

// TODO: Multiple nested blocks are shown badly (because of indenting
// as I wanted to avoid newline with '{' symbol.
std::string BlockStmtAST::dump(int level) const {
    //std::string res = getStrWithIndent(level) + "{\n";
    std::string res = "{\n";
    for (auto &cmd : m_cmds) {
        res += cmd->dump(level) + "\n";
    }
    res += getStrWithIndent(level-1) + "}";
    return res;
}

std::string ExpressionStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    res += m_expr->dump() + ";";
    return res;
}

std::string IfStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    res += "if (";
    res += m_condExpr->dump() + ") ";
    if (m_thenStmt->stmt_type() != STMT_TYPE::BLOCK)
        res += "\n" + m_thenStmt->dump(level+1);
    else res += m_thenStmt->dump(level+1);
    return res;
}

std::string AssignmentStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    if (m_type != VLANG_TYPE::NO_VAR_DECL) res += to_str(m_type) + " ";
    res += m_varName + " = " + m_expr->dump() + ";";
    return res;
}

} // ;vlang
