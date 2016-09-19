/*
 * TypeChecker.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "Expression.hpp"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/// -----------------------------------------------------------------------------------------------
/// \brief Performs the type checking.
/// -----------------------------------------------------------------------------------------------
class TypeChecker {
public:
    static bool checkTypes(ExprAST* a, ExprAST* b) {
        if (a->type() == b->type())
            return true;
        else return false;
    }

    static bool checkTypesWithDiagnostics(ExprAST* a, ExprAST* b) {
        if (a->type() == b->type())
            return true;
        //std::cerr << "Operand 1 has type: " << (a->type() == nullptr ? "VOID" : a->type()->str()) << std::endl;
        //std::cerr << "Operand 2 has type: " << (b->type() == nullptr ? "VOID" : b->type()->str()) << std::endl;
        //std::cerr << "Type mismatch detected." << std::endl;
        return false;
    }
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* ifndef TYPE_CHECKER_HPP */
