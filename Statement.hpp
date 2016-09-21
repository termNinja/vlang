/*
 * Statement.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "LLVMCodegen.hpp"
#include "Expression.hpp"
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/// -----------------------------------------------------------------------------------------------
/// \brief Used to fast discover a class type in class hierarchy.
/// -----------------------------------------------------------------------------------------------
typedef enum {
    RETURN, BLOCK, IF, IF_ELSE, WHILE, FOR, ASSIGNMENT,
    ASSIGNMENT_LIST, PROTOTYPE, FUNCTION, EXPRESSION, EMPTY
} STMT_TYPE;

/// -----------------------------------------------------------------------------------------------
/// \brief An abstract statement class.
/// -----------------------------------------------------------------------------------------------
class StmtAST {
public:
    virtual ~StmtAST() {}

    /// \brief Returns a string representation of Vala statement.
    virtual std::string dump(int level = 0) const = 0;

    /// \brief Returns an enum representing the type of statement inside class hierarchy.
    virtual STMT_TYPE stmt_type() const = 0;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents a return statement.
/// -----------------------------------------------------------------------------------------------
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

/// -----------------------------------------------------------------------------------------------
/// \brief A block expression.
/// -----------------------------------------------------------------------------------------------
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
    const std::vector<StmtAST*>& blockStatements() const { return m_cmds; }

private:
    std::vector<StmtAST*> m_cmds;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents the assignment statement.
///
/// If variable to which it's assigned is of type NO_VAR_DECL
///     it means that variable has already been declared and
///     we're assigning it a new value.
/// -----------------------------------------------------------------------------------------------
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
    bool isAllowed() const;

private:
    VLANG_TYPE m_type;
    std::string m_varName;
    ExprAST* m_expr;
};

class AssignmentListStmtAST : public StmtAST {
public:
    AssignmentListStmtAST (VLANG_TYPE type, std::vector<std::pair<std::string, ExprAST*>> assignmentList)
        : m_type(type), m_list(assignmentList)
    {}
    ~AssignmentListStmtAST () {
        for (auto &elem : m_list)
            delete elem.second;
    }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::ASSIGNMENT_LIST; }
    std::unique_ptr<std::vector<bool>> isAllowed() const;

private:
    VLANG_TYPE m_type;
    std::vector<std::pair<std::string, ExprAST*>> m_list;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents some constant evaluation (remove later)
/// -----------------------------------------------------------------------------------------------
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

class EmptyStmtAST : public StmtAST {
public:
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::EMPTY; }
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an if statement (with no else).
/// -----------------------------------------------------------------------------------------------
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

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an if-then-else statement.
/// -----------------------------------------------------------------------------------------------
class IfElseStmtAST : public StmtAST {
public:
    IfElseStmtAST(ExprAST* condExpr, StmtAST* thenStmt, StmtAST* elseStmt)
        : m_condExpr(condExpr), m_thenStmt(thenStmt), m_elseStmt(elseStmt)
    {}
    ~IfElseStmtAST() {
        delete m_condExpr;
        delete m_thenStmt;
        delete m_elseStmt;
    }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::IF_ELSE; }

private:
    ExprAST* m_condExpr;
    StmtAST* m_thenStmt;
    StmtAST* m_elseStmt;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents an while statement.
/// -----------------------------------------------------------------------------------------------
class WhileStmtAST : public StmtAST {
public:
    WhileStmtAST(ExprAST* condExpr, StmtAST* bodyStmt)
        : m_condExpr(condExpr), m_bodyStmt(bodyStmt)
    {}
    ~WhileStmtAST() {
        delete m_condExpr;
        delete m_bodyStmt;
    }
    std::string dump(int level = 0) const;
    STMT_TYPE stmt_type() const { return STMT_TYPE::WHILE; }

private:
    ExprAST* m_condExpr;
    StmtAST* m_bodyStmt;
};

// TODO:
// This class shall be dealt with later once I start compiling onto
// LLVM IR. I leave it because LLVM requires a lot of information
// like return value in order to generated a 'call' instruction.
// class CallFunStmtAST : public StmtAST {
// public:
//     CallFunStmtAST ();
//     ~CallFunStmtAST ();
//
// private:
//     std::string m_name;
// };

/// \brief This class is a simple placeholder for both PrototypeAST and FunctionAST.
class ProtoDefContainer : public StmtAST {
public:
    virtual std::string name() const = 0;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represents a function declaration.
/// -----------------------------------------------------------------------------------------------
class PrototypeAST : public ProtoDefContainer {
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

/// -----------------------------------------------------------------------------------------------
/// \brief Represents a function with a definition.
/// -----------------------------------------------------------------------------------------------
class FunctionAST : public ProtoDefContainer {
public:
    FunctionAST(PrototypeAST proto, BlockStmtAST* definition)
        : m_proto(proto), m_definition(definition)
    {}
    ~FunctionAST() { delete m_definition; }
    const PrototypeAST& proto() const { return m_proto; }
    std::string dump(int level = 0) const;
    std::string name() const { return m_proto.name(); }
    STMT_TYPE stmt_type() const { return STMT_TYPE::FUNCTION; }
    const BlockStmtAST* body() const { return m_definition; }

private:
    PrototypeAST m_proto;
    BlockStmtAST* m_definition;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* ifndef STATEMENT_HPP */

