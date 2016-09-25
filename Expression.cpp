/*
 * Expression.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Expression.hpp"
#include "GlobalContainers.hpp"
#include "color.h"

#include "ProgramOptions.hpp"

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

// TODO
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

// TODO
Value* UnaryExprAST::codegen() const {
    std::cerr << "UnaryExprAST::codegen() has not yet been implemented!" << std::endl;
    return nullptr;
}

Value* BinaryExprAST::codegen() const {
    if (m_op == "=") {
        Value* assignMe = m_right->codegen();
        if (! assignMe) return logError("Failed m_right->codegen() in BinaryExprAST::codegen()");
        if (m_left->exp_type() != EXP_TYPE::VARIABLE_EXP) return logError("Bad left operand in assignment, it isnt a variable!");
        VariableExprAST* var = static_cast<VariableExprAST*>(m_left);

        // Check if it's a local variable
        auto localFinder = NamedValues.find(var->name());
        if (localFinder != NamedValues.end())
            return Builder.CreateStore(assignMe, localFinder->second);

        // Check if global
        auto globalFinder = GlobalValues.find(var->name());
        if (globalFinder != GlobalValues.end()) return Builder.CreateStore(assignMe, globalFinder->second);

        return logError("Failed assigning to variable '" + var->name() + "'");
    }
    Value* left = m_left->codegen();
    Value* right = m_right->codegen();
    if (left == nullptr) return logError("Failed m_left->codegen() in BinaryExprAST::codegen()");
    if (right == nullptr) return logError("Failed m_right->codegen() in BinaryExprAST::codegen()");

    if (m_op == "+") {
        switch (type()->vlang_type()) {
            case VLANG_TYPE::INT32: return Builder.CreateAdd(left, right, "intadd");
            case VLANG_TYPE::DOUBLE: return Builder.CreateFAdd(left, right, "doubleadd");
            default:
                std::cerr << "Unsupported operation + on operands of type: " << type()->str() << std::endl;
                return nullptr;
        }
    } else if (m_op == "-") {
        switch (type()->vlang_type()) {
            case VLANG_TYPE::INT32: return Builder.CreateSub(left, right, "intsub");
            case VLANG_TYPE::DOUBLE: return Builder.CreateFSub(left, right, "doublesub");
            default:
                std::cerr << "Unsupported operation - on operands of type: " << type()->str() << std::endl;
                return nullptr;
        }
    } else if (m_op == "*") {
        switch (type()->vlang_type()) {
            case VLANG_TYPE::INT32: return Builder.CreateMul(left, right, "intmul");
            case VLANG_TYPE::DOUBLE: return Builder.CreateFMul(left, right, "doublemul");
            default:
                std::cerr << "Unsupported operation * on operands of type: " << type()->str() << std::endl;
                return nullptr;
        }
    } else if (m_op == "/") {
        switch (type()->vlang_type()) {
            case VLANG_TYPE::INT32: return Builder.CreateUDiv(left, right, "intdiv");
            case VLANG_TYPE::DOUBLE: return Builder.CreateFDiv(left, right, "doublediv");
            default:
                std::cerr << "Unsupported operation / on operands of type: " << type()->str() << std::endl;
                return nullptr;
        }
    } else if (m_op == "%") {
        if (type()->vlang_type() == VLANG_TYPE::INT32) {
            return Builder.CreateSRem(left, right, "intmod");
        } else {
            std::cerr << "Unsupported operation " << m_op << " on operands of type: " << type()->str() << std::endl;
            return nullptr;
        }
    } else if (m_op == "<") {
        switch (type()->vlang_type()) {
            case VLANG_TYPE::INT32:
                return Builder.CreateICmpULT(left, right, "intlt");
            case VLANG_TYPE::DOUBLE:
                left = Builder.CreateFCmpULT(left, right, "doublelt");
                return Builder.CreateUIToFP(left, LLVM_DOUBLETY(), "boollt");
            default:
                std::cerr << "Unsupported operation " << m_op << " on operands of type: " << type()->str() << std::endl;
                return nullptr;
        }
    } else if (m_op == ">") {
        switch (type()->vlang_type()) {
            case VLANG_TYPE::INT32:
                return Builder.CreateICmpUGT(left, right, "intgt");
            case VLANG_TYPE::DOUBLE:
                left = Builder.CreateFCmpUGT(left, right, "doublegt");
                return Builder.CreateUIToFP(left, LLVM_DOUBLETY(), "boolgt");
            default:
                std::cerr << "Unsupported operation " << m_op << " on operands of type: " << type()->str() << std::endl;
                return nullptr;
        }
    }
    return nullptr;
}

Value* FunctionCallExprAST::codegen() const {
    Function* f = GetFunction(m_name);
    if (f == nullptr) return logError("Failed finding function " + m_name);
    if (m_args.size() != f->arg_size()) return logError("Wrong number of arguments!");

    // Create arguments
    std::vector<Value*> args;
    for (auto & arg : m_args) args.push_back(arg->codegen());
    return Builder.CreateCall(f, args, "calltmp");
}

Value* BoolExprAST::codegen() const {
    return LLVM_BOOL(m_val);
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
        if (m_left->type()->strength() > m_right->type()->strength())
            return m_left->type();
        else return m_right->type();
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
    return new FunctionCallExprAST(m_name, args, m_retType);
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Dump functions
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
std::string ConstIntExprAST::dump(unsigned) const {
    std::string res = boost::lexical_cast<std::string>(m_val);
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(INT_C) + res + std::string(RESET);
    return res;
}

std::string ConstDoubleExprAST::dump(unsigned) const {
    std::string res = std::to_string(m_val);
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(DOUBLE_C) + res + std::string(RESET);
    return res;
}

std::string StringExprAST::dump(unsigned) const {
    std::string res = "\"" + m_str + "\"";
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(STRING_C) + res + std::string(RESET);
    return res;
}

std::string UnaryExprAST::dump(unsigned) const {
    std::string res = m_op;
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(OPERATOR_C) + res + std::string(RESET);
    res += m_expr->dump();
    return res;
}

std::string BinaryExprAST::dump(unsigned) const {
    std::string res = m_op;
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(OPERATOR_C) + res + std::string(RESET);
    return m_left->dump() + " " + res + " " + m_right->dump();
}

std::string VariableExprAST::dump(unsigned) const {
    std::string res = boost::lexical_cast<std::string>(m_name);
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(VARIABLE_C) + res + std::string(RESET);
    return res;
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
    std::string res = (m_val == true ? "true" : "false");
    if (vlang::util::ProgramOptions::get().syntax_highlight())
        res = std::string(BOOL_C) + res + std::string(RESET);
    return res;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
