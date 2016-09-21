/*
 * SemanticAnalyzer.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "SemanticAnalyzer.hpp"
#include "color.h"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
namespace semant {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

bool SemanticAnalyzer::isAllowedAssignment(VLANG_TYPE variableType, VLANG_TYPE exprType) {
    if (variableType == VLANG_TYPE::DOUBLE && exprType == VLANG_TYPE::INT32) return true;
    if (variableType != exprType) return false;
    else return true;
}

std::vector<StmtAST*>* SemanticAnalyzer::performAnalysis() {
    std::cerr << BOLDRED << DRAGON_SEPARATOR() << RESET << std::endl;
    std::cerr << BOLDBLUE << "Semantic analysis initiated." << RESET << std::endl;
    std::cerr << BOLDRED << DRAGON_SEPARATOR() << RESET << std::endl;

    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    //      Semantic analysis runs       //
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //

    // Cast types into required values
    typeCastRun();

    // Perform type checking
    unsigned int numberOfErrors = 0;
    if (! typeCheckRun(&numberOfErrors)) {
        std::cerr << "Found " << numberOfErrors << " errors. " << std::endl;
        std::cerr << BOLDRED << "fatal error: " << RED << " Can't proceed with compilation."
                  << RESET << std::endl;
    } else {
        reportSuccess("Type check was successful.");
    }

    // Insert other runs here


    std::cerr << BOLDRED << DRAGON_SEPARATOR() << RESET << std::endl;
    std::cerr << BOLDBLUE << "Semantic analysis finished." << RESET << std::endl;
    std::cerr << BOLDRED << DRAGON_SEPARATOR() << RESET << std::endl;
    return m_ast;
}

void SemanticAnalyzer::reportSuccess(std::string msg) const {
    std::cerr << BOLDGREEN << "[semant]: " << msg << RESET << std::endl;
}

void SemanticAnalyzer::typeCastRun() {

}

void SemanticAnalyzer::reportAssignmentError(std::string err) const {
    std::cerr << RED << err << RESET << std::endl;
}

// TODO: Prettier error reporting (once I conclude everything works well)
bool SemanticAnalyzer::typeCheckRun(unsigned int* numberOfErrors) {
    bool allFine = true;

    // For each program statement, like function definition, declaration...
    for (auto& programStatement : *m_ast) {
        // So far, we only check functions definitions
        if (programStatement->stmt_type() == STMT_TYPE::FUNCTION) {
            // We iterate over each statement inside function body
            auto functionBody = static_cast<FunctionAST*>(programStatement)->body()->blockStatements();
            for (auto &stmt : functionBody) {
                if (stmt->stmt_type() == STMT_TYPE::ASSIGNMENT) {
                    if (! static_cast<AssignmentStmtAST*>(stmt)->isAllowed()) {
                        (*numberOfErrors)++;
                        allFine = false;
                    } else {
                    }
                } else if (stmt->stmt_type() == STMT_TYPE::ASSIGNMENT_LIST) {
                    std::unique_ptr<std::vector<bool>> res = static_cast<AssignmentListStmtAST*>(stmt)->isAllowed();
                    for (unsigned i = 0; i < res->size(); ++i) {
                        if (! (*res)[i]) {
                            // i-th assignment in assignment list has an error
                            reportAssignmentError("Error in assignment: " + std::to_string(i));
                            allFine = false;
                            (*numberOfErrors)++;
                            std::cerr << "Setting allFine to false AssignmentListStmtAST" << std::endl;
                        }
                    }
                }
            }
        }
    }

    return allFine;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;semant
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


