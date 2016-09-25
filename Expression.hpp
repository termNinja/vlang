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
/// \brief Used to fast discover a class type in class hierarchy.
/// -----------------------------------------------------------------------------------------------
typedef enum {
    INT_EXP, DOUBLE_EXP, STRING_EXP, VARIABLE_EXP, BINARY_EXP, UNARY_EXP, CALL_EXP
} EXP_TYPE;

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an abstract expression.
/// -----------------------------------------------------------------------------------------------
class ExprAST {
public:
    virtual ~ExprAST() {}

    /// \brief Returns a string containing vala source code.
    virtual std::string dump(unsigned level = 0) const = 0;

    /// \brief Returns the type of the expression.
    virtual const VlangType* type() const = 0;

    /// \brief Returns an LLVM Value* node.
    virtual Value* codegen() const = 0;

    /// \brief Returns an enum representing the type of expression inside class hierarchy.
    virtual EXP_TYPE exp_type() const = 0;

    /// \brief Returns a pointer to a promoted version of expression in regards to its type.
    /// For example, if we have int + double, promotion would give double + double.
    ///
    /// NOTE: Still working on it.
    virtual ExprAST* promote(VLANG_TYPE type) = 0;

    /// \brief Performs casting of given node into a given type.
    /// What happens with binary expressions? Casts both operands into given type.
    virtual ExprAST* convertTo(VLANG_TYPE type) = 0;

    /// \brief Returns a clone of the current node (recursively clones)
    virtual ExprAST* clone() const = 0;
};

// TODO: Still to make a decision on this
std::pair<int, VLANG_TYPE> DetermineExpressionConversion(const ExprAST* left, const ExprAST* right);

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST string constant node.
/// -----------------------------------------------------------------------------------------------
class StringExprAST : public ExprAST {
public:
    StringExprAST(std::string str)
        : m_str(str), m_type(new StringType())
    {}
    ~StringExprAST() {
        delete m_type;
    }
    std::string val() const { return m_str; }

    virtual std::string dump(unsigned level = 0) const;
    virtual const VlangType* type() const { return m_type; }
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::STRING_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        // TODO
        return nullptr;
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    std::string m_str;
    VlangType* m_type;
};


/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST double const node.
/// -----------------------------------------------------------------------------------------------
class ConstDoubleExprAST : public ExprAST {
public:
    ConstDoubleExprAST(double val)
        : m_val(val), m_type(new DoubleType())
    {}
    ~ConstDoubleExprAST() {
        delete m_type;
    }
    double val() const { return m_val; }

    virtual std::string dump(unsigned level = 0) const;
    virtual const VlangType* type() const { return m_type; }
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::DOUBLE_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        if (type == VLANG_TYPE::INT32 || type == VLANG_TYPE::DOUBLE)
            return nullptr;     // no promotion needed
        else if (type == VLANG_TYPE::STRING)
            return new StringExprAST(std::to_string(m_val));
        else {
            std::cerr << "Type not yet supported: '" << to_str(type) << "'" << std::endl;
            return nullptr;
        }
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    double m_val;
    VlangType* m_type;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST int const node.
// TODO: Switch it to LONG LONG it in order to be able to represent smaller int types.
// TODO: Allow signed, unsigned values
/// -----------------------------------------------------------------------------------------------
class ConstIntExprAST : public ExprAST {
public:
    ConstIntExprAST(int val)
        : m_val(val), m_type(new Int32Type())
    {}
    ~ConstIntExprAST() {
        delete m_type;
    }
    int val() const { return m_val; }

    virtual std::string dump(unsigned level = 0) const;
    virtual const VlangType* type() const { return m_type; }
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::INT_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        if (type == VLANG_TYPE::INT32) return nullptr;
        else if (type == VLANG_TYPE::DOUBLE)
            return new ConstDoubleExprAST(m_val);
        else if (type == VLANG_TYPE::STRING)
            return new StringExprAST(std::to_string(m_val));
        else {
            std::cerr << "Type not yet supported: '" << to_str(type) << "'" << std::endl;
            return this;
        }
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    int m_val;
    VlangType* m_type;
};

class BoolExprAST : public ExprAST {
public:
    BoolExprAST(bool val)
        : m_val(val), m_type(new BoolType())
    {}
    ~BoolExprAST() {
        delete m_type;
    }
    bool val() const { return m_val; }

    virtual std::string dump(unsigned level = 0) const;
    virtual const VlangType* type() const { return m_type; }
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::STRING_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        return nullptr;
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    bool m_val;
    VlangType* m_type;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an AST variable node.
/// -----------------------------------------------------------------------------------------------
class VariableExprAST : public ExprAST {
public:
    VariableExprAST(std::string name, VLANG_TYPE type)
        : m_name(name), m_type(make_from_enum(type))
    {}
    VariableExprAST(std::string name)
        : m_name(name)
    {}
    ~VariableExprAST() {
        delete m_type;
    }
    std::string name() const { return m_name; }

    virtual const VlangType* type() const { return m_type; }
    virtual std::string dump(unsigned level = 0) const;
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::VARIABLE_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        if (m_type->vlang_type() == type) return nullptr;
        else {
            // TODO: Finsih this with an external method that will determine required type
            return nullptr;
        }
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    std::string m_name;
    VlangType* m_type;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents a function call expression.
/// Comment: I passed type as argument because it was difficult to avoid mutual inclusion of
/// headers files in order to access global map that uses Statement and Statement uses Expression.
/// -----------------------------------------------------------------------------------------------
class FunctionCallExprAST : public ExprAST {
public:
    FunctionCallExprAST(std::string name, std::vector<ExprAST*> args, VLANG_TYPE retType)
        : m_name(name), m_args(args), m_retType(retType)
    {}
    virtual std::string dump(unsigned level = 0) const;
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::CALL_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        return nullptr;
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;
    virtual const VlangType* type() const {
        return make_from_enum(m_retType);
    }

private:
    std::string m_name;
    std::vector<ExprAST*> m_args;
    VLANG_TYPE m_retType;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an unary expression.
/// -----------------------------------------------------------------------------------------------
class UnaryExprAST : public ExprAST {
public:
    UnaryExprAST(std::string operation, ExprAST* operand)
        : m_op(operation), m_expr(operand)
    {}
    ~UnaryExprAST() { delete m_expr; }
    std::string operation() const { return m_op; }

    virtual const VlangType* type() const {
        return m_expr->type();
    }
    virtual std::string dump(unsigned level = 0) const;
    virtual Value* codegen() const;
    virtual EXP_TYPE exp_type() const { return EXP_TYPE::UNARY_EXP; }
    virtual ExprAST* promote(VLANG_TYPE type) {
        // TODO: later
        return nullptr;
    }
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    std::string m_op;
    ExprAST* m_expr;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an binary expression.
/// -----------------------------------------------------------------------------------------------
class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(std::string operation, ExprAST* left, ExprAST* right)
        : m_op(operation), m_left(left), m_right(right)
    {}
    ~BinaryExprAST() {
        delete m_left;
        delete m_right;
    }
    std::string operation() const { return m_op; }

    virtual EXP_TYPE exp_type() const { return EXP_TYPE::BINARY_EXP; }
    virtual const VlangType* type() const;
    virtual std::string dump(unsigned level = 0) const;
    virtual Value* codegen() const;
    virtual ExprAST* promote(VLANG_TYPE type);
    virtual ExprAST* convertTo(VLANG_TYPE type);
    virtual ExprAST* clone() const;

private:
    std::string m_op;
    ExprAST* m_left;
    ExprAST* m_right;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* ifndef EXPRESSION_HPP */
