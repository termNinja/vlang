#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "LLVMCodegen.hpp"
#include <boost/lexical_cast.hpp>
#include <exception>

namespace boost{
	void throw_exception(std::exception const &e){}
}

namespace vlang {

class ExprAST {
public:
	virtual ~ExprAST() {}
	virtual std::string dump() const = 0;
};

class IntConstExprAST : public ExprAST {
public:
	IntConstExprAST(int val, unsigned int size = 32)
		: m_val(val), m_size(size)
	{}
	virtual std::string dump() const;
	int val() const { return m_val; }

private:
	int m_val;
	unsigned int m_size;
};

class DoubleConstExprAST : public ExprAST {
public:
	DoubleConstExprAST(double val)
		: m_val(val)
	{}
	virtual std::string dump() const;
	double val() const { return m_val; }

private:
	double m_val;
};


} // ;vlang

#endif /* ifndef EXPRESSION_HPP */
