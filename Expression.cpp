/*
 * Expression.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Expression.hpp"
#include "GlobalContainers.hpp"

// Required in order to use lexical cast
namespace boost{
    void throw_exception(std::exception const &e){(void)(e);}
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Codegen functions (LLVM related)
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Value* ConstIntExprAST::codegen() const {
    return LLVM_INT(m_val);
}

Value* ConstDoubleExprAST::codegen() const {
    return LLVM_DOUBLE(m_val);
}

Value* StringExprAST::codegen() const {
    return nullptr;
}

Value* VariableExprAST::codegen() const {
    AllocaInst* varAddress = NamedValues[m_name];
    if (! varAddress) {
        GlobalValue* gVar = GlobalValues[m_name];
        if (! gVar) return logError("Unknown variable: '" + m_name + "'");
        return Builder.CreateLoad(gVar);
    }
    return Builder.CreateLoad(varAddress);
}

Value* UnaryExprAST::codegen() const {
    return nullptr;
}

Value* BinaryExprAST::codegen() const {
    // TODO: Finish this once I complete semantic analysis.

    return nullptr;
}

Value* FunctionCallExprAST::codegen() const {
    return nullptr;
}

std::pair<int, VLANG_TYPE> DetermineExpressionConversion(const ExprAST* left, const ExprAST* right) {
    if (left->type()->strength() > right->type()->strength()) {
        return std::pair<int, VLANG_TYPE>(2, left->type()->vlang_type());
    }
    else  {
        return std::pair<int, VLANG_TYPE>(1, right->type()->vlang_type());
    }
}

const VlangType* BinaryExprAST::type() const {
    if (m_left->type() == m_right->type())
        return m_left->type();
    else {
        std::cerr << "[BinaryExprAST]: Operand type mismatch." << std::endl;
        std::cerr << "[BinaryExprAST]: left: " << m_left->type()->str() << std::endl;
        std::cerr << "[BinaryExprAST]: right: " << m_right->type()->str() << std::endl;
        return m_left->type();
    }
}

// TODO: bad bad bad.
// WHat to return in case a promotion occurs?
// I can't return nullptr => I return this or m_left or m_right
// If I return this, it gets deleted (hellno!)
// If I return old m_left, it can work, but it's ugly
ExprAST* BinaryExprAST::promote(VLANG_TYPE) {
    if (m_left->type()->vlang_type() == m_right->type()->vlang_type())
        return nullptr;
    std::pair<int, VLANG_TYPE> to_conv = DetermineExpressionConversion(m_left, m_right);
    if (to_conv.first == 1) {
        // we convert the left operand
        ExprAST* new_left = m_left->promote(to_conv.second);
        delete m_left;
        m_left = new_left;
    } else if (to_conv.first == 2) {
        // we convert the right operand
        ExprAST* new_right = m_right->promote(to_conv.second);
        delete m_right;
        m_right = new_right;
    } else {
        std::cerr << "Fatal error detected in BinaryExprAST::promote()" << std::endl;
        return nullptr;
    }
    return nullptr;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Dump functions
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
std::string ConstIntExprAST::dump(unsigned) const {
    return boost::lexical_cast<std::string>(m_val);
}

std::string ConstDoubleExprAST::dump(unsigned) const {
    return boost::lexical_cast<std::string>(m_val);
}

std::string StringExprAST::dump(unsigned) const {
    return "\"" + m_str + "\"";
}

std::string UnaryExprAST::dump(unsigned) const {
    return m_op + m_expr->dump();
}

std::string BinaryExprAST::dump(unsigned) const {
    return m_left->dump() + " " + m_op + " " + m_right->dump();
}

std::string VariableExprAST::dump(unsigned) const {
    return boost::lexical_cast<std::string>(m_name);
}

std::string FunctionCallExprAST::dump(unsigned) const {
    // We try to find the function
    ProtoDefContainer* ZiFunction = FunctionContainer[m_name];
    unsigned i = 0;
    if (ZiFunction == nullptr) {
        std::cout << "Failed finding function '" << m_name << "'" << std::endl;
        return "";
    } else {
        std::string res = ZiFunction->name() + "(";
        if (m_args.empty()) return res += ")";
        for (; i < m_args.size()-1; ++i)
            res += m_args[i]->dump() + ", ";
        res += m_args[i]->dump() + ")";
        return res;
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
