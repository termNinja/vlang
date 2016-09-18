/*
 * Types.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Types.hpp"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// INT32
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string Int32Type::str() const {
    return "int";
}

VLANG_TYPE Int32Type::vlang_type() const {
    return VLANG_TYPE::INT32;
}

Type* Int32Type::llvm_type() const {
    return LLVM_INTTY;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DOUBLE TYPE
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string DoubleType::str() const {
    return "double";
}

VLANG_TYPE DoubleType::vlang_type() const {
    return VLANG_TYPE::DOUBLE;
}

Type* DoubleType::llvm_type() const {
    return LLVM_DOUBLETY;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// STRING TYPE
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string StringType::str() const {
    return "string";
}

Type* StringType::llvm_type() const {
    std::cerr << "TOOD: StringType::llvm_type()" << std::endl;
    return nullptr;
}

VLANG_TYPE StringType::vlang_type() const {
    return VLANG_TYPE::STRING;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// UTILS
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string to_str(VLANG_TYPE type) {
    switch (type) {
        case INT32:
            return "int";
        case INT64:
            return "int64";
        case DOUBLE:
            return "double";
        case BOOL:
            return "bool";
        case STRING:
            return "string";
        case VOID:
            return "void";
        default:
            return "unknown_type";
    }
}

std::string to_str(Type* llvm_type) {
    if (llvm_type == LLVM_INTTY)
        return "int";
    else if (llvm_type == LLVM_DOUBLETY)
        return "double";
    else
        return "unknown_type";
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
