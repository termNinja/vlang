/*
 * SemanticAnalyzer.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "SemanticAnalyzer.hpp"
#include "color.h"
#include "ProgramOptions.hpp"

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

    unsigned int numberOfErrors = 0;

    // Cast types into required values
    typeCastRun();

    // --------------- //
    //  Typechecking   //
    // --------------- //
    if (! typeCheckRun(&numberOfErrors))
        std::cerr << BOLDRED << "fatal error: " << RESET << " errors: "
                  << BOLDWHITE << numberOfErrors  << RESET << std::endl;
    else reportSuccess("Type check was successful.");

    // ------------ //
    // UNKNOWN TYPE //
    // ------------ //

    // Insert other runs here
    // TODO:
    // * Variable already declared


    std::cerr << BOLDRED << DRAGON_SEPARATOR() << RESET << std::endl;
    std::cerr << BOLDBLUE << "Semantic analysis finished." << RESET << std::endl;
    std::cerr << BOLDRED << DRAGON_SEPARATOR() << RESET << std::endl;
    return m_ast;
}

void SemanticAnalyzer::reportSuccess(std::string msg) const {
    std::cerr << BOLDGREEN << msg << RESET << std::endl;
}

void SemanticAnalyzer::typeCastRun() {

}

void SemanticAnalyzer::reportAssignmentError(std::string err) const {
    std::cerr << RED << err << RESET << std::endl;
}

void SemanticAnalyzer::reportAssignmentError(std::string statement, unsigned long long line, VLANG_TYPE left, VLANG_TYPE right) const {
    std::cerr << util::ProgramOptions::get().first_input_file() << ":" << line << ":" << BOLDRED << " error:" << RESET;
    std::cerr << " Assignment: Cannot convert from " << BOLDWHITE << "'" << to_str(right) << "'" << RESET
              << " to " << BOLDWHITE << "'" << to_str(left) << "'" << std::endl;
    std::cerr << statement << RESET << std::endl << std::endl;
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
                        std::pair<VLANG_TYPE, VLANG_TYPE> t = static_cast<AssignmentStmtAST*>(stmt)->assignmentTypes();
                        reportAssignmentError(stmt->dump(), stmt->line(), t.first, t.second);
                        allFine = false;
                    } else { /* assignment is correct */ }
                } else if (stmt->stmt_type() == STMT_TYPE::ASSIGNMENT_LIST) {
                    AssignmentListStmtAST* ptr = static_cast<AssignmentListStmtAST*>(stmt);
                    std::unique_ptr<std::vector<bool>> res = ptr->isAllowed();
                    for (unsigned i = 0; i < res->size(); ++i) {
                        if (! (*res)[i]) {
                            // i-th assignment in assignment list has an error
                            std::pair<VLANG_TYPE, VLANG_TYPE> t = ptr->assignmentTypesIth(i);
                            reportAssignmentError(ptr->dump(), ptr->line(), t.first, t.second);
                            allFine = false;
                            (*numberOfErrors)++;
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


