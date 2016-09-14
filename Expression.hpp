#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "LLVMCodegen.hpp"
#include "Types.hpp"
#include <boost/lexical_cast.hpp>
#include <exception>
#include <vector>

namespace vlang {

class ExprAST {
public:
	virtual ~ExprAST() {}
	virtual std::string dump() const = 0;
	virtual VLANG_TYPE type() const = 0;
	virtual Type* llvm_type() const = 0;
};

/// Expression which type is int
/// (for example, a constant, a int variable, a function which returns an int...)
class IntExprAST : public ExprAST {
public:
	VLANG_TYPE type() const { return VLANG_TYPE::INT32; }
	Type* llvm_type() const { return LLVM_INTTY; }
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// INT EXPRESSIONS
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/// Represents an AST int const node
class IntConstExprAST : public IntExprAST {
public:
	IntConstExprAST(int val, unsigned int size = 32)
		: m_val(val), m_size(size)
	{}
	virtual std::string dump() const;
	int val() const { return m_val; }
	int size() const { return m_size; }

private:
	int m_val;
	unsigned int m_size;
};

class UnaryIntExprAST : public IntExprAST {
public:
	UnaryIntExprAST(std::string operation, IntExprAST* operand)
		: m_operation(operation), m_operand(operand)
	{}
	~UnaryIntExprAST() { delete m_operand; }

	std::string operation() const { return m_operation; }
	virtual std::string dump() const;

private:
	std::string m_operation;
	IntExprAST* m_operand;
};

class PrototypeIntExprAST {
public:
	PrototypeIntExprAST(std::string name, std::vector<std::string> args)
		: m_name(name), m_args(args)
	{}
	Value* codegen() const;

private:
	std::string m_name;
	std::vector<std::string> m_args;
};

class BinaryIntExprAST : public IntExprAST {
public:
	BinaryIntExprAST(std::string operation, IntExprAST* left, IntExprAST* right)
		: m_operation(operation), m_left(left), m_right(right)
	{}
	~BinaryIntExprAST() {
		delete m_left;
		delete m_right;
	}
	std::string operation() const { return m_operation; }
	virtual std::string dump() const;

private:
	std::string m_operation;
	IntExprAST* m_left;
	IntExprAST* m_right;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DOUBLE EXPRESSIONS
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/// Represents an AST double const node
class DoubleConstExprAST : public ExprAST {
public:
	DoubleConstExprAST(double val)
		: m_val(val)
	{}
	virtual std::string dump() const;
	double val() const { return m_val; }
	VLANG_TYPE type() const { return VLANG_TYPE::DOUBLE; }
	Type* llvm_type() const { return LLVM_DOUBLETY; }

private:
	double m_val;
};

/// Represents an AST variable const node
class VariableExprAST : public ExprAST {
public:
	VariableExprAST(std::string name, VLANG_TYPE type)
		: m_name(name), m_type(type)
	{}
	VLANG_TYPE type() const { return m_type; }
	std::string name() const { return m_name; }
	virtual std::string dump() const;
private:
	std::string m_name;
	VLANG_TYPE m_type;
};


} // ;vlang

#endif /* ifndef EXPRESSION_HPP */
