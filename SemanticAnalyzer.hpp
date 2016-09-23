/*
 * SemanticAnalyzer.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include <vector>
#include <map>
#include "Expression.hpp"
#include "Statement.hpp"
#include "GlobalContainers.hpp"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
namespace semant {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


/// \brief Performs semantic analysis.
/// Does NOT deallocate given AST (because obviously it's needed after it finishes working)
/// It can also change AST (for example, double x = 1 -> double x = 1.0)
class SemanticAnalyzer {
public:
    SemanticAnalyzer(std::vector<StmtAST*>* ast)
        : m_ast(ast)
    {}
    ~SemanticAnalyzer() {}

    /// \brief Performs semantic analysis and returns modified AST
    std::vector<StmtAST*>* performAnalysis();

    /// \brief Checks if an assignment is allowed.
    static bool isAllowedAssignment(VLANG_TYPE variableType, VLANG_TYPE exprType);

private:
    /// \brief Functions traverses the AST and does some basic upcasting.
    /// For example: double x = 1; where 1 is an int will get transformed into:
    /// double x = 1.0 where 1.0 is an double.
    void typeCastRun();

    /// \brief Traverses the AST and performs type checking. Reports errors if found.
    /// \return Returns true if compilation can proceed further.
    bool typeCheckRun(unsigned int* numberOfErrrors);

    /// \brief Reports an assignment error with given error message.
    void reportAssignmentError(std::string err) const;

    /// \brief Reports an assignment error with given error message and on given line.
    void reportAssignmentError(std::string statement, unsigned long long line, VLANG_TYPE left, VLANG_TYPE right) const;

    /// \brief Reports an sucessful operation with given message.
    void reportSuccess(std::string msg) const;

    std::vector<StmtAST*>* m_ast;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;semant
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* !SEMANTIC_ANALYZER_HPP */
