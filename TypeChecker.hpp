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
};

} // ;vlang

#endif /* ifndef TYPE_CHECKER_HPP */
