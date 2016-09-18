#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "Expression.hpp"

namespace vlang {

/// This class performs the type checking
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
        std::cerr << "Operand 1 has type: " << to_str(a->type()) << std::endl;
        std::cerr << "Operand 2 has type: " << to_str(b->type()) << std::endl;
        std::cerr << "Type mismatch detected." << std::endl;
        return false;
    }
};

} // ;vlang

#endif /* ifndef TYPE_CHECKER_HPP */
