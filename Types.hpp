#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include "LLVMCodegen.hpp"

namespace vlang {

typedef enum{
	INT32, INT64, DOUBLE, BOOL, STRING, VOID
}VLANG_TYPE;

std::string to_str(VLANG_TYPE type);
std::string to_str(Type* llvm_type);

} //;vlang

#endif /* ifndef TYPES_HPP */
