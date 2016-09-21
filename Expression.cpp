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
// CastTo
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ExprAST* ConstIntExprAST::convertTo(VLANG_TYPE type) {
    switch (type) {
    case VLANG_TYPE::INT32:     return this->clone();
    case VLANG_TYPE::DOUBLE:    return new ConstDoubleExprAST(m_val);
    case VLANG_TYPE::STRING:    return new StringExprAST(std::to_string(m_val));
    case VLANG_TYPE::BOOL:      return new BoolExprAST(m_val);
    default:
        std::cerr << "Got strange arg in ConstIntExprAST::convertTo(VLANG_TYPE)" << std::endl;
        return nullptr;
    }
}

ExprAST* ConstDoubleExprAST::convertTo(VLANG_TYPE type) {
    switch (type) {
    case VLANG_TYPE::INT32:     return new ConstIntExprAST(m_val);
    case VLANG_TYPE::DOUBLE:    return this->clone();
    case VLANG_TYPE::STRING:    return new StringExprAST(std::to_string(m_val));
    case VLANG_TYPE::BOOL:      return new BoolExprAST(m_val);
    default:
        std::cerr << "Got strange arg in ConstDoubleExprAST::convertTo(VLANG_TYPE)" << std::endl;
        return nullptr;
    }
}

ExprAST* BoolExprAST::convertTo(VLANG_TYPE type) {
    switch (type) {
    case VLANG_TYPE::INT32:     return new ConstIntExprAST(m_val == true ? 1 : 0);
    case VLANG_TYPE::DOUBLE:    return new ConstDoubleExprAST(m_val == true ? 1.0 : 0.0);
    case VLANG_TYPE::STRING:    return new StringExprAST(std::to_string(m_val));
    case VLANG_TYPE::BOOL:      return this->clone();
    default:
        std::cerr << "Got strange arg in ConstDoubleExprAST::convertTo(VLANG_TYPE)" << std::endl;
        return nullptr;
    }
}

// TODO
ExprAST* VariableExprAST::convertTo(VLANG_TYPE type) {
    return nullptr;
}

ExprAST* StringExprAST::convertTo(VLANG_TYPE type) {
    switch (type) {
    case VLANG_TYPE::INT32:     return new ConstIntExprAST(atoi(m_str.c_str()));
    case VLANG_TYPE::DOUBLE:    return new ConstDoubleExprAST(atof(m_str.c_str()));
    case VLANG_TYPE::STRING:    return this->clone();
    case VLANG_TYPE::BOOL:      return new BoolExprAST(m_str == "true" ? true : false);
    default:
        std::cerr << "Got strange arg in StringExprAST::convertTo(VLANG_TYPE)" << std::endl;
        return nullptr;
    }
}

// TODO
ExprAST* UnaryExprAST::convertTo(VLANG_TYPE type) {
    return new UnaryExprAST(m_op, m_expr->convertTo(type));
}

// TODO
ExprAST* BinaryExprAST::convertTo(VLANG_TYPE type) {
    return new BinaryExprAST(m_op, m_left->convertTo(type), m_right->convertTo(type));
}

ExprAST* FunctionCallExprAST::convertTo(VLANG_TYPE type) {
    switch (type) {
    case VLANG_TYPE::INT32:     return nullptr;
    case VLANG_TYPE::DOUBLE:    return nullptr;
    case VLANG_TYPE::STRING:    return nullptr;
    case VLANG_TYPE::BOOL:      return nullptr;
    default:
        std::cerr << "Got strange arg in FunctionCallExprAST::convertTo(VLANG_TYPE)" << std::endl;
        return nullptr;
    }
}
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

Value* BoolExprAST::codegen() const {
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
// Clone
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ExprAST* ConstIntExprAST::clone() const { return new ConstIntExprAST(m_val); }
ExprAST* ConstDoubleExprAST::clone() const { return new ConstDoubleExprAST(m_val); }
ExprAST* BoolExprAST::clone() const { return new BoolExprAST(m_val); }
ExprAST* StringExprAST::clone() const { return new StringExprAST(m_str); }
ExprAST* VariableExprAST::clone() const { return new VariableExprAST(m_name); }
ExprAST* UnaryExprAST::clone() const { return new UnaryExprAST(m_op, m_expr->clone()); }
ExprAST* BinaryExprAST::clone() const { return new BinaryExprAST(m_op, m_left->clone(), m_right->clone()); }
ExprAST* FunctionCallExprAST::clone() const {
    std::vector<ExprAST*> args;
    for (auto &a : m_args) args.push_back(a->clone());
    return new FunctionCallExprAST(m_name, args);
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

std::string BoolExprAST::dump(unsigned) const {
    if (m_val) return "true";
    else return "false";
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
