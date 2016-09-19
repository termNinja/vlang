/*
 * Expression.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <exception>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "LLVMCodegen.hpp"
#include "Types.hpp"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an abstract expression.
/// -----------------------------------------------------------------------------------------------
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual std::string dump(unsigned level = 0) const = 0;
    virtual VLANG_TYPE type() const = 0;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST int const node.
// TODO: Switch it to LONG LONG it in order to be able to represent smaller int types.
// TODO: Allow signed, unsigned values
/// -----------------------------------------------------------------------------------------------
class ConstIntExprAST : public ExprAST {
public:
    ConstIntExprAST(int val)
        : m_val(val)
    {}
    int val() const { return m_val; }

    virtual std::string dump(unsigned level = 0) const;
    virtual VLANG_TYPE type() const { return VLANG_TYPE::INT32; }

private:
    int m_val;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST double const node.
/// -----------------------------------------------------------------------------------------------
class ConstDoubleExprAST : public ExprAST {
public:
    ConstDoubleExprAST(double val)
        : m_val(val)
    {}
    double val() const { return m_val; }

    virtual std::string dump(unsigned level = 0) const;
    virtual VLANG_TYPE type() const { return VLANG_TYPE::DOUBLE; }

private:
    double m_val;
    DoubleType* m_type;
};

class StringExprAST : public ExprAST {
public:
    StringExprAST(std::string str)
        : m_str(str)
    {}
    std::string val() const { return m_str; }

    virtual std::string dump(unsigned level = 0) const;
    virtual VLANG_TYPE type() const { return VLANG_TYPE::STRING; }

private:
    std::string m_str;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST variable node.
/// -----------------------------------------------------------------------------------------------
class VariableExprAST : public ExprAST {
public:
    VariableExprAST(std::string name, VLANG_TYPE type)
        : m_name(name), m_type(type)
    {}
    std::string name() const { return m_name; }

    virtual VLANG_TYPE type() const { return m_type; }
    virtual std::string dump(unsigned level = 0) const;

private:
    std::string m_name;
    VLANG_TYPE m_type;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an unary expression.
/// -----------------------------------------------------------------------------------------------
class UnaryExprAST : public ExprAST {
public:
    UnaryExprAST(std::string operation, ExprAST* operand)
        : m_operation(operation), m_operand(operand)
    {}
    ~UnaryExprAST() { delete m_operand; }
    std::string operation() const { return m_operation; }

    virtual std::string dump(unsigned level = 0) const;
    virtual VLANG_TYPE type() const {
        return m_operand->type();
    }

private:
    std::string m_operation;
    ExprAST* m_operand;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an binary expression.
/// -----------------------------------------------------------------------------------------------
class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(std::string operation, ExprAST* left, ExprAST* right)
        : m_operation(operation), m_left(left), m_right(right)
    {}
    ~BinaryExprAST() {
        delete m_left;
        delete m_right;
    }
    std::string operation() const { return m_operation; }

    virtual std::string dump(unsigned level = 0) const;
    virtual VLANG_TYPE type() const {
        if (m_left->type() == m_right->type())
            return m_left->type();
        else {
            std::cerr << "[BinaryExprAST]: Operand type mismatch." << std::endl;
            std::cerr << "[BinaryExprAST]: left: " << to_str(m_left->type()) << std::endl;
            std::cerr << "[BinaryExprAST]: right: " << to_str(m_right->type()) << std::endl;
            return m_left->type();
        }
    }

private:
    std::string m_operation;
    ExprAST* m_left;
    ExprAST* m_right;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* ifndef EXPRESSION_HPP */
