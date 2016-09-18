#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <iostream>
#include "LLVMCodegen.hpp"

namespace vlang {

typedef enum{
    INT32, INT64, DOUBLE, BOOL, STRING, VOID, NO_VAR_DECL
}VLANG_TYPE;

std::string to_str(VLANG_TYPE type);
std::string to_str(Type* llvm_type);

// Classes are not yet utilized inside the compiler.
// For now, I have decided to stick with a simple enum for types
// because really, it's more then enough.
class VlangType {
public:
    virtual std::string str() const = 0;
    virtual ~VlangType() {}
    virtual Type* llvm_type() const = 0;
    virtual VLANG_TYPE vlang_type() const = 0;
};

class Int32Type : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
};

class DoubleType : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
};

class StringType : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
};


} //;vlang

#endif /* ifndef TYPES_HPP */
