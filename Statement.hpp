#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "LLVMCodegen.hpp"
#include "Expression.hpp"
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

namespace vlang {

typedef enum {
    RETURN, BLOCK, IF, ASSIGNMENT, PROTOTYPE, FUNCTION, EXPRESSION
} STMT_TYPE;

class StmtAST {
public:
    virtual ~StmtAST() {}
    virtual std::string dump(int level = 0) const = 0;
    virtual STMT_TYPE stmt_type() const = 0;
};

class ReturnStmtAST : public StmtAST {
public:
    ReturnStmtAST(ExprAST* retVal)
        : m_retVal(retVal)
    {}
    ~ReturnStmtAST() { delete m_retVal; }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::RETURN; }

private:
    ExprAST* m_retVal;
};

/// \brief Represents a block expression.
class BlockStmtAST : public StmtAST {
public:
    BlockStmtAST(std::vector<StmtAST*> cmds)
        : m_cmds(cmds)
    {}
    ~BlockStmtAST() {
        for (auto &cmd : m_cmds) delete cmd;
    }
    std::string dump(int level = 0) const;
    VLANG_TYPE type() const { return VLANG_TYPE::VOID; }
    STMT_TYPE stmt_type() const { return STMT_TYPE::BLOCK; }

private:
    std::vector<StmtAST*> m_cmds;
};

/// \brief Represents the assignment statement.
/// If variable to which it's assigned is of type NO_VAR_DECL
///     it means that variable has already been declared and
///     we're assigning it a new value.
class AssignmentStmtAST : public StmtAST {
public:
    AssignmentStmtAST(VLANG_TYPE type, std::string varName, ExprAST* expr)
        : m_type(type), m_varName(varName), m_expr(expr)
    {}
    ~AssignmentStmtAST() {
        delete m_expr;
    }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::ASSIGNMENT; }

private:
    VLANG_TYPE m_type;
    std::string m_varName;
    ExprAST* m_expr;
};

/// \brief Represents some constant evaluation (remove later)
class ExpressionStmtAST : public StmtAST {
public:
    ExpressionStmtAST(ExprAST* expr)
        : m_expr(expr)
    {}
    ~ExpressionStmtAST() { delete m_expr; }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::EXPRESSION; }

private:
    ExprAST* m_expr;
};

/// \brief Represents an if statement (with no else).
class IfStmtAST : public StmtAST {
public:
    IfStmtAST(ExprAST* condExpr, StmtAST* thenStmt)
        : m_condExpr(condExpr), m_thenStmt(thenStmt)
    {}
    ~IfStmtAST() {
        delete m_condExpr;
        delete m_thenStmt;
    }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::IF; }

private:
    ExprAST* m_condExpr;
    StmtAST* m_thenStmt;
};

/// \brief Represents a function declaration.
class PrototypeAST {
public:
    PrototypeAST(std::string name, VLANG_TYPE retVal, std::vector<std::pair<VLANG_TYPE, std::string>> args)
        : m_name(name), m_retVal(retVal), m_args(args)
    {}
    std::string dump(int level = 0) const;
    std::string name() const { return m_name; }
    VLANG_TYPE ret_val_type() const { return m_retVal; }
    STMT_TYPE stmt_type() const { return STMT_TYPE::PROTOTYPE; }

private:
    std::string m_name;
    VLANG_TYPE m_retVal;
    std::vector<std::pair<VLANG_TYPE, std::string>> m_args;
};

/// \brief Represents a function with a definition.
class FunctionAST {
public:
    FunctionAST(PrototypeAST proto, BlockStmtAST* definition)
        : m_proto(proto), m_definition(definition)
    {}
    ~FunctionAST() { delete m_definition; }
    const PrototypeAST& proto() const { return m_proto; }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::FUNCTION; }

private:
    PrototypeAST m_proto;
    BlockStmtAST* m_definition;
};

} // ;vlang

#endif /* ifndef STATEMENT_HPP */

