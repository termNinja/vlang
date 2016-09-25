/*
 * Types.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Types.hpp"
#include "ProgramOptions.hpp"
#include "color.h"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// INT32
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string Int32Type::str() const {
    std::string res = "int";
    if (util::ProgramOptions::get().syntax_highlight())
        res = std::string(TYPE_C) + res + std::string(RESET);
    return res;
}

VLANG_TYPE Int32Type::vlang_type() const {
    return VLANG_TYPE::INT32;
}

Type* Int32Type::llvm_type() const {
    return LLVM_INTTY();
}

int Int32Type::strength() const {
    return 20;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DOUBLE TYPE
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string DoubleType::str() const {
    std::string res = "double";
    if (util::ProgramOptions::get().syntax_highlight())
        res = std::string(TYPE_C) + res + std::string(RESET);
    return res;
}

VLANG_TYPE DoubleType::vlang_type() const {
    return VLANG_TYPE::DOUBLE;
}

Type* DoubleType::llvm_type() const {
    return LLVM_DOUBLETY();
}

int DoubleType::strength() const {
    return 30;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// STRING TYPE
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string StringType::str() const {
    return "string";
}

Type* StringType::llvm_type() const {
    std::cerr << "TODO: StringType::llvm_type()" << std::endl;
    return nullptr;
}

VLANG_TYPE StringType::vlang_type() const {
    return VLANG_TYPE::STRING;
}

int StringType::strength() const {
    return 40;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// BOOL TYPE
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string BoolType::str() const {
    std::string res = "bool";
    if (util::ProgramOptions::get().syntax_highlight())
        res = std::string(TYPE_C) + res + std::string(RESET);
    return res;
}

Type* BoolType::llvm_type() const {
    return LLVM_BOOLTY();
}

VLANG_TYPE BoolType::vlang_type() const {
    return VLANG_TYPE::BOOL;
}

int BoolType::strength() const {
    return 10;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// VOID
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string VoidType::str() const {
    std::string res = "void";
    if (util::ProgramOptions::get().syntax_highlight())
        res = std::string(TYPE_C) + res + std::string(RESET);
    return res;
}

Type* VoidType::llvm_type() const {
    return LLVM_VOIDTY();
}

VLANG_TYPE VoidType::vlang_type() const {
    return VLANG_TYPE::VOID;
}

int VoidType::strength() const {
    return -10;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// UTILS
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string to_str(VLANG_TYPE type) {
    std::string res;
    switch (type) {
        case INT32:     res = "int";        break;
        case INT64:     res = "int64";      break;
        case DOUBLE:    res = "double";     break;
        case BOOL:      res = "bool";       break;
        case STRING:    res = "string";     break;
        case VOID:      res = "void";       break;
        default:        res = "unknown_t";  break;
    }
    if (util::ProgramOptions::get().syntax_highlight())
        return std::string(TYPE_C) + res + std::string(RESET);
    else return res;
}

// TODO: Support syntax highlight here? Not really needed atm...
std::string to_str(Type* llvm_type) {
    if (llvm_type == LLVM_INTTY())
        return "int";
    else if (llvm_type == LLVM_DOUBLETY())
        return "double";
    else
        return "unknown_type";
}

VlangType* make_from_enum(VLANG_TYPE type) {
    switch (type) {
        case VLANG_TYPE::INT32:
            return new Int32Type();
        case VLANG_TYPE::DOUBLE:
            return new DoubleType();
        case VLANG_TYPE::STRING:
            return new StringType();
        case VLANG_TYPE::BOOL:
            return new BoolType();
        case VLANG_TYPE::VOID:
            return new VoidType();
        default:
            std::cerr << "What is this type? " << to_str(type) << std::endl;
            return nullptr;
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
