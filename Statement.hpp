#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "LLVMCodegen.hpp"
#include "Expression.hpp"
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

namespace vlang {

class StmtAST {
public:
	virtual ~StmtAST() {}
	virtual std::string dump(unsigned level = 0) const = 0;
};

class ReturnStmtAST : public StmtAST {
public:
	ReturnStmtAST(ExprAST* retVal)
		: m_retVal(retVal)
	{}
	~ReturnStmtAST() { delete m_retVal; }
	std::string dump(unsigned level = 0) const;

private:
	ExprAST* m_retVal;
};

class PrototypeAST {
public:
	PrototypeAST(std::string name, VLANG_TYPE retVal, std::vector<std::pair<VLANG_TYPE, std::string>> args)
		: m_name(name), m_retVal(retVal), m_args(args)
	{}
	std::string dump(unsigned level = 0) const;
	std::string name() const { return m_name; }
	VLANG_TYPE ret_val_type() const { return m_retVal; }

private:
	std::string m_name;
	VLANG_TYPE m_retVal;
	std::vector<std::pair<VLANG_TYPE, std::string>> m_args;
};

} // ;vlang

#endif /* ifndef STATEMENT_HPP */

