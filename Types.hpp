/*
 * Types.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <iostream>
#include "LLVMCodegen.hpp"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

typedef enum{
    INT32, INT64, DOUBLE, BOOL, STRING, VOID, NO_VAR_DECL, UNKNOWN
} VLANG_TYPE;

std::string to_str(VLANG_TYPE type);
std::string to_str(Type* llvm_type);

// NOTE
// Classes are not yet utilized inside the compiler.
// For now, I have decided to stick with a simple enum for types
// because really, it's more then enough.

/// -----------------------------------------------------------------------------------------------
/// \brief Represenets an abstract vlang type.
/// -----------------------------------------------------------------------------------------------
class VlangType {
public:
    virtual std::string str() const = 0;
    virtual ~VlangType() {}
    virtual Type* llvm_type() const = 0;
    virtual VLANG_TYPE vlang_type() const = 0;
    virtual int strength() const = 0;
};

// We redefine the operator for comparisons in order to lesses the code bloat.
inline bool operator==(const VlangType& left, const VlangType& right) {
    return left.vlang_type() == right.vlang_type();
}
inline bool operator!=(const VlangType& left, const VlangType& right) {
    return left.vlang_type() != right.vlang_type();
}

/// -----------------------------------------------------------------------------------------------
/// \brief Represenets an int32 type.
/// -----------------------------------------------------------------------------------------------
class Int32Type : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
    virtual int strength() const;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represenets a double type.
/// -----------------------------------------------------------------------------------------------
class DoubleType : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
    virtual int strength() const;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represenets an string type.
/// -----------------------------------------------------------------------------------------------
class StringType : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
    virtual int strength() const;
};

/// -----------------------------------------------------------------------------------------------
/// \brief Represenets an bool type.
/// -----------------------------------------------------------------------------------------------
class BoolType : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
    virtual int strength() const;
};

class VoidType : public VlangType {
public:
    virtual std::string str() const;
    virtual Type* llvm_type() const;
    virtual VLANG_TYPE vlang_type() const;
    virtual int strength() const;
};

VlangType* make_from_enum(VLANG_TYPE type);

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} //;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* ifndef TYPES_HPP */
